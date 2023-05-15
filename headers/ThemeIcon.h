#ifndef THEMEICON_H
#define THEMEICON_H
#pragma once

#include <QObject>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>

class ThemeIcon : public QObject
{
  Q_OBJECT

public:
  static void setIcon(QToolButton &button, QString path);
  static void setIcon(QLabel &label, QString path);
  static QIcon getIcon(QString path);
};

#endif // THEMEICON_H
