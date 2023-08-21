#include "ThemeIcon.h"

void ThemeIcon::setIcon(QToolButton &button, QString path)
{
  std::thread t([&button, path]()
  {
    button.setIcon(getIcon(path, button.iconSize()));
  });

  t.detach();
}

void ThemeIcon::setIcon(QLabel &label, QString path)
{
  std::thread t([&label, path]()
  {
    label.setPixmap(getIcon(path, label.size()).pixmap(label.size()));
  });

  t.detach();
}

QIcon ThemeIcon::getIcon(QString path, QSize size)
{
  QIcon icon(path);

  if (QPalette().color(QPalette::Window).value() < 128)
  {
    QPixmap pixmap = icon.pixmap(icon.actualSize(size + QSize(16, 16)));
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    icon = QIcon(pixmap);
  }

  return icon;
}
