#include "VideoPlayer.h"
#include <QBuffer>
#include <QImageWriter>
#include <QDebug>

VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent) {
  m_player.setVideoOutput(&m_sink);
  connect(&m_sink, &QVideoSink::videoFrameChanged, this, &VideoPlayer::onVideoFrame);
}

void VideoPlayer::setPluginManager(PluginManager *pm) {
  m_pluginManager = pm;
}

void VideoPlayer::setSource(const QString &file) {
  m_player.setSource(QUrl::fromLocalFile(file));
}

void VideoPlayer::play() { m_player.play(); }
void VideoPlayer::pause() { m_player.pause(); }
void VideoPlayer::seek(int ms) { m_player.setPosition(ms); }

void VideoPlayer::onVideoFrame(const QVideoFrame &frame) {
  if (!frame.isValid()) return;
  QImage img = frame.toImage();
  if (img.isNull()) return;
  if (m_pluginManager) {
    img = m_pluginManager->processFrame(img);
  }
  QByteArray bytes;
  QBuffer buf(&bytes);
  buf.open(QIODevice::WriteOnly);
  img.save(&buf, "PNG");
  emit frameReady(bytes);
}
