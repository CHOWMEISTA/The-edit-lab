#pragma once
#include <QObject>
#include <QImage>
#include <QString>

class IFeature {
public:
  virtual ~IFeature() = default;
  virtual QString id() const = 0;
  virtual QString name() const = 0;
  virtual void activate() = 0;
  virtual void deactivate() = 0;
  virtual QImage process(const QImage &frame) = 0;
};

using CreateFeatureFn = IFeature* (*)();
using DestroyFeatureFn = void (*)(IFeature*);
