#include "MainWindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDir>
#include <QFile>
#include <QFontDatabase>
#include <QStyleFactory>

//Creates folders in the current directory
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
  application.setApplicationVersion(APP_VERSION "-beta");

  //Creating a folder with configuration files
  {
    QString path = QDir::homePath();

    #if defined(Q_OS_WIN)
    for (const QString &key : QStyleFactory::keys())
    {
      if (key.toLower() == "fusion")
      {
        application.setStyle(QStyleFactory::create(key));
      }
    }
    #endif

    #if defined(Q_OS_LINUX)
    #if FLATPAK
    path.append("/.var/app/" APP_ID "/config");
    #else
    path.append("/.config/");
    path.append(PROJECT_NAME);
    #endif
    #elif defined(Q_OS_WIN)
    path.append("/AppData/Local/");
    path.append(PROJECT_NAME);
    #endif

    createPath({path});
    QDir::setCurrent(path);
  }

  //Translation of the application
  {
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
  }

  //Setup the application font
  {
    QString fontFileName = ":/fonts/Roboto-Regular.ttf";
    QFont font("Roboto", 11);
    int fontId = QFontDatabase::addApplicationFont(fontFileName);
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

    if (fontFamilies.size() > 0)
    {
      font.setFamily(fontFamilies.first());
      application.setFont(font);
    }
  }

  createPath({"Chat"});
  MainWindow windows;
  QString mainStyle;
  QFile style(":/style.css");

  if (style.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    mainStyle = style.readAll();
  }

  windows.setStyleSheet(mainStyle);
  windows.show();
  return application.exec();
}
