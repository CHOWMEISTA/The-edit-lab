#pragma once
#include <QObject>

class Timeline : public QObject {
  Q_OBJECT
public:
  explicit Timeline(QObject *parent = nullptr);
  // Minimal placeholder: expand as needed for multi-track / clip model
};
