#pragma once
#include <QObject>
#include <QMediaPlayer>
#include <QVideoSink>
#include <QVideoFrame>
#include <QImage>
#include "PluginManager.h"

class VideoPlayer : public QObject {
  Q_OBJECT
public:
  explicit VideoPlayer(QObject *parent = nullptr);
  void setPluginManager(PluginManager *pm);
  Q_INVOKABLE void setSource(const QString &file);
  Q_INVOKABLE void play();
  Q_INVOKABLE void pause();
  Q_INVOKABLE void seek(int ms);

signals:
  void frameReady(const QByteArray &pngData);

private slots:
  void onVideoFrame(const QVideoFrame &frame);

private:
  QMediaPlayer m_player;
  QVideoSink m_sink;
  PluginManager *m_pluginManager = nullptr;
};
