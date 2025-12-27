#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QFileInfo>
#include <QDir>
#include <QWebEngineView>
#include <QWebChannel>

#include "Bridge.h"
#include "PluginManager.h"
#include "VideoPlayer.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  QCoreApplication::setApplicationName("Qt Timeline Editor");

  // Core components
  PluginManager *pm = new PluginManager();
  VideoPlayer *vp = new VideoPlayer();
  Bridge *bridge = new Bridge();
  bridge->setPluginManager(pm);
  bridge->setVideoPlayer(vp);

  // Main window layout
  QMainWindow win;
  QWidget *central = new QWidget();
  QHBoxLayout *layout = new QHBoxLayout(central);
  QSplitter *split = new QSplitter();

  // Preview: native QLabel (JS side can also display via base64)
  QLabel *preview = new QLabel();
  preview->setMinimumSize(480, 270);
  preview->setAlignment(Qt::AlignCenter);
  split->addWidget(preview);

  // Web UI
  QWebEngineView *view = new QWebEngineView();
  QString uiPath = QDir::current().filePath("ui/index.html");
  view->setUrl(QUrl::fromLocalFile(uiPath));
  split->addWidget(view);

  layout->addWidget(split);
  win.setCentralWidget(central);

  // WebChannel
  QWebChannel *channel = new QWebChannel(view->page());
  channel->registerObject(QStringLiteral("bridge"), bridge);
  view->page()->setWebChannel(channel);

  // Auto-load plugins from ./plugins (if any were built and copied)
  QDir pluginsDir(QCoreApplication::applicationDirPath());
  pluginsDir.cd("plugins");
  if (pluginsDir.exists()) {
    const auto entries = pluginsDir.entryList(QDir::Files);
    qDebug() << "Found plugin files:" << entries;
    for (const QString &file : entries) {
      const QString path = pluginsDir.filePath(file);
      qDebug() << "Attempting to load plugin:" << path;
      // attempt to load; PluginManager will warn if it fails
      pm->loadPlugin(path);
    }
  }
  // connect frames to preview pixmap
  QObject::connect(vp, &VideoPlayer::frameReady, [&](const QByteArray &png){
    QPixmap pmx;
    pmx.loadFromData(png, "PNG");
    preview->setPixmap(pmx.scaled(preview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // also emit via bridge to JS
    QMetaObject::invokeMethod(bridge, "onFrameReady", Q_ARG(QByteArray, png));
  });

  win.resize(1200, 720);
  win.show();

  return app.exec();
}
