#include "ThemeIcon.h"

void ThemeIcon::setIcon(QToolButton &button, QString path)
{
	button.setIcon(getIcon(path));
}

void ThemeIcon::setIcon(QLabel &label, QString path)
{
	label.setPixmap(getIcon(path).pixmap(32, 32));
}

QIcon ThemeIcon::getIcon(QString path)
{
	QImage image(path);
	image.convertTo(QImage::Format_ARGB32);

	for (int y = 0; y < image.height(); y++)
	{
		QRgb *scanLine = (QRgb *)image.scanLine(y);

		for (int x = 0; x < image.width(); x++)
		{
			QRgb pixel = *scanLine;
			quint8 color;

			if (QPalette().color(QPalette::Window).value() < 128)
			{
				color = 255;
			}
			else
			{
				color = 0;
			}

			*scanLine = qRgba(color, color, color, qAlpha(pixel));
			scanLine++;
		}
	}

	return QPixmap::fromImage(image);
}
