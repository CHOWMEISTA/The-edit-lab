#pragma once
#include <QObject>
#include <QVariantList>
#include <QByteArray>
#include <QImage>
#include "PluginManager.h"
#include "VideoPlayer.h"

class Bridge : public QObject {
  Q_OBJECT
public:
  explicit Bridge(QObject *parent = nullptr);

  void setPluginManager(PluginManager *pm);
  void setVideoPlayer(VideoPlayer *vp);

  Q_INVOKABLE QVariantList getPlugins();
  Q_INVOKABLE bool loadPlugin(const QString &path);
  Q_INVOKABLE void togglePlugin(const QString &id, bool on);

  Q_INVOKABLE void setSource(const QString &file);
  Q_INVOKABLE void play();
  Q_INVOKABLE void pause();
  Q_INVOKABLE void seek(int ms);

  Q_INVOKABLE void exportRange(int startMs, int endMs, const QString &inputPath, const QString &outPath);

signals:
  void pluginListUpdated(const QVariantList &list);
  void frameReady(const QString &base64Png);

private slots:
  void onFrameReady(const QByteArray &pngBytes);

private:
  PluginManager *m_pm = nullptr;
  VideoPlayer *m_vp = nullptr;
};
