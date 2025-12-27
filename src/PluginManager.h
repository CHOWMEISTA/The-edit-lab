#pragma once
#include <QObject>
#include <QLibrary>
#include <QVariantList>
#include <QImage>
#include <QMap>
#include "IFeature.h"

struct PluginInfo {
  QString id;
  QString name;
  QString path;
  QLibrary *lib = nullptr;
  IFeature *instance = nullptr;
  DestroyFeatureFn destroyFn = nullptr;
  bool enabled = false;
};

class PluginManager : public QObject {
  Q_OBJECT
public:
  explicit PluginManager(QObject *parent = nullptr);
  ~PluginManager();

  Q_INVOKABLE QVariantList pluginList() const;
  Q_INVOKABLE bool loadPlugin(const QString &path);
  Q_INVOKABLE bool unloadPlugin(const QString &id);
  Q_INVOKABLE bool togglePlugin(const QString &id, bool on);
  Q_INVOKABLE bool reloadPlugin(const QString &id);
  QImage processFrame(const QImage &frame);

signals:
  void pluginListUpdated(const QVariantList &list);

private:
  QMap<QString, PluginInfo> m_plugins;
};
