#include "IFeature.h"
#include <QImage>

class GrayscaleFeature : public IFeature {
public:
  QString id() const override { return "grayscale"; }
  QString name() const override { return "Grayscale"; }
  void activate() override {}
  void deactivate() override {}
  QImage process(const QImage &frame) override {
    if (frame.format() != QImage::Format_Grayscale8)
      return frame.convertToFormat(QImage::Format_Grayscale8);
    return frame;
  }
};

extern "C" IFeature* createFeature() { return new GrayscaleFeature(); }
extern "C" void destroyFeature(IFeature* f) { delete f; }
