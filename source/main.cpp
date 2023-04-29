#include "MainWindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDir>
#include <QFile>
#include <QFontDatabase>
#include <QStyleFactory>
#include <QtGlobal>

void createPath(QStringList pathList)
{
	for (QString path : pathList)
	{
		path = QDir::currentPath() + "/" + path;
		if(!QDir(path).exists())
		{
			QDir(path).mkdir(path);
		}
	}
}

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	application.setApplicationVersion("0.3.1-beta");

	#if defined(Q_OS_WIN)
			QApplication::setStyle(QStyleFactory::create("Fusion"));
	#endif

	QTranslator translator;
	const QStringList uiLanguages = QLocale::system().uiLanguages();

	for (const QString &locale : uiLanguages)
	{
		const QString baseName = "MindMate_" + QLocale(locale).name();

		if (translator.load(":/i18n/" + baseName))
		{
			application.installTranslator(&translator);
			break;
		}
	}


	QString fontFileName = ":/fonts/Roboto-Regular.ttf";
	QFont font("Roboto", 11);
	int fontId = QFontDatabase::addApplicationFont(fontFileName);
	QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

	if (fontFamilies.size() > 0)
	{
		font.setFamily(fontFamilies.at(0));
		QApplication::setFont(font);
	}

	createPath({"Chat"});
	MainWindow windows;
	QString mainStyle;
	mainStyle =
	"QScrollBar:vertical {\
		width: 8px;\
		border-radius: 4px;}\
	QScrollBar:vertical:hover {\
		border-radius: 0px;\
		background-color: #313131;}\
	QScrollBar::handle:vertical {\
		border-radius: 4px;\
		border: 1px solid #020202;\
		background-color: #a4a4a3;}\
	QScrollBar::handle:vertical:hover {\
		background-color: #c9c9c7;}\
	QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, \
	QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\
		background: none;}";
	windows.setStyleSheet(mainStyle);
	windows.show();
	return application.exec();
}
