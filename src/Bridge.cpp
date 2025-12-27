#include "Bridge.h"
#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QProcess>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

Bridge::Bridge(QObject *parent) : QObject(parent) {}

void Bridge::setPluginManager(PluginManager *pm) { m_pm = pm; connect(m_pm, &PluginManager::pluginListUpdated, this, &Bridge::pluginListUpdated); }
void Bridge::setVideoPlayer(VideoPlayer *vp) { m_vp = vp; connect(m_vp, &VideoPlayer::frameReady, this, &Bridge::onFrameReady); }

QVariantList Bridge::getPlugins() { return m_pm ? m_pm->pluginList() : QVariantList(); }
bool Bridge::loadPlugin(const QString &path) {
  if (!m_pm) return false;
  return m_pm->loadPlugin(path);
}
void Bridge::togglePlugin(const QString &id, bool on) {
  if (!m_pm) return;
  m_pm->togglePlugin(id, on);
}

void Bridge::setSource(const QString &file) { if (m_vp) m_vp->setSource(file); }
void Bridge::play() { if (m_vp) m_vp->play(); }
void Bridge::pause() { if (m_vp) m_vp->pause(); }
void Bridge::seek(int ms) { if (m_vp) m_vp->seek(ms); }

void Bridge::onFrameReady(const QByteArray &pngBytes) {
  QString b64 = QString::fromLatin1(pngBytes.toBase64());
  emit frameReady(b64);
}

void Bridge::exportRange(int startMs, int endMs, const QString &inputPath, const QString &outPath) {
  // call ffmpeg.exe CLI for the MVP: re-encode with libx264 + aac
  qint64 startMsec = startMs;
  qint64 durationMsec = endMs - startMs;
  double startSec = startMsec / 1000.0;
  double durSec = durationMsec / 1000.0;
  QStringList args = {
    "-y",
    "-ss", QString::number(startSec),
    "-i", inputPath,
    "-t", QString::number(durSec),
    "-c:v", "libx264",
    "-preset", "veryfast",
    "-c:a", "aac",
    outPath
  };
  QProcess proc;
  proc.start("ffmpeg", args);
  proc.waitForFinished(-1);
}
