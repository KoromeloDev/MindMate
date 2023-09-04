#ifndef THEMEICON_H
#define THEMEICON_H

#include <QObject>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <thread>

class ThemeIcon : public QObject
{
  Q_OBJECT

public:
  static void setIcon(QToolButton &button, QString path);
  static void setIcon(QLabel &label, QString path);
  static QIcon getIcon(QString path, QSize size = QSize(32, 32));
};

#endif // THEMEICON_H
