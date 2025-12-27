#include "PluginManager.h"
#include <QFileInfo>
#include <QDebug>

PluginManager::PluginManager(QObject *parent) : QObject(parent) {}

PluginManager::~PluginManager() {
  for (auto it = m_plugins.begin(); it != m_plugins.end(); ++it) {
    auto &p = it.value();
    if (p.instance && p.destroyFn) p.destroyFn(p.instance);
    if (p.lib) {
      p.lib->unload();
      delete p.lib;
    }
  }
}

QVariantList PluginManager::pluginList() const {
  QVariantList list;
  for (auto it = m_plugins.constBegin(); it != m_plugins.constEnd(); ++it) {
    QVariantMap m;
    m["id"] = it.key();
    m["name"] = it.value().name;
    m["enabled"] = it.value().enabled;
    list.append(m);
  }
  return list;
}

bool PluginManager::loadPlugin(const QString &path) {
  QFileInfo fi(path);
  if (!fi.exists()) {
    qWarning() << "Plugin not found:" << path;
    return false;
  }

  QLibrary *lib = new QLibrary(path);
  if (!lib->load()) {
    qWarning() << "Failed to load plugin:" << lib->errorString();
    delete lib;
    return false;
  }

  auto createFn = (CreateFeatureFn)lib->resolve("createFeature");
  auto destroyFn = (DestroyFeatureFn)lib->resolve("destroyFeature");
  if (!createFn || !destroyFn) {
    qWarning() << "Plugin does not export required functions";
    lib->unload();
    delete lib;
    return false;
  }

  IFeature *inst = createFn();
  if (!inst) {
    qWarning() << "createFeature() returned null";
    lib->unload();
    delete lib;
    return false;
  }

  PluginInfo info;
  info.id = inst->id();
  info.name = inst->name();
  info.path = path;
  info.lib = lib;
  info.instance = inst;
  info.destroyFn = destroyFn;
  info.enabled = false;

  m_plugins.insert(info.id, info);
  emit pluginListUpdated(pluginList());
  return true;
}

bool PluginManager::unloadPlugin(const QString &id) {
  if (!m_plugins.contains(id)) return false;
  PluginInfo p = m_plugins.take(id);
  if (p.instance && p.destroyFn) p.destroyFn(p.instance);
  if (p.lib) {
    p.lib->unload();
    delete p.lib;
  }
  emit pluginListUpdated(pluginList());
  return true;
}

bool PluginManager::togglePlugin(const QString &id, bool on) {
  if (!m_plugins.contains(id)) return false;
  PluginInfo &p = m_plugins[id];
  if (p.enabled == on) return true;
  p.enabled = on;
  if (on) p.instance->activate(); else p.instance->deactivate();
  emit pluginListUpdated(pluginList());
  return true;
}

bool PluginManager::reloadPlugin(const QString &id) {
  if (!m_plugins.contains(id)) return false;
  PluginInfo p = m_plugins[id];
  unloadPlugin(id);
  return loadPlugin(p.path);
}

QImage PluginManager::processFrame(const QImage &frame) {
  QImage cur = frame;
  for (auto it = m_plugins.constBegin(); it != m_plugins.constEnd(); ++it) {
    if (it.value().enabled && it.value().instance) {
      cur = it.value().instance->process(cur);
    }
  }
  return cur;
}
