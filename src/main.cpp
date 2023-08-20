#include "MainWindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDir>
#include <QFile>
#include <QFontDatabase>
#include <QStyleFactory>
#include <thread>

//Setup the application font
void setFont()
{
  QString fontFileName = ":/resources/fonts/Roboto-Regular.ttf";
  QFontDatabase::addApplicationFont(fontFileName);
  QApplication::setFont(QFont("Roboto", 11));
}

//Translation of the application
void setTranslation(QTranslator &translator)
{
  const QVector<QString> uiLanguages = QLocale::system().uiLanguages();

  for (const QString &locale : uiLanguages)
  {
    const QString baseName = "MindMate_" + QLocale(locale).name();

    if (translator.load(":/i18n/" + baseName))
    {
      QApplication::installTranslator(&translator);
      break;
    }
  }
}

//Creates folders in the current directory
void createPath(const QVector<QString> &pathList)
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

//Creating a folder with configuration files
void setPath()
{
  QString path;

  #if defined(Q_OS_LINUX)
  path.append(QDir::homePath());

  #if FLATPAK
  path.append("/.var/app/");
  path.append(APP_ID);
  path.append("/config");
  #else
  path.append("/.config/");
  path.append(PROJECT_NAME);
  #endif
  #elif defined(Q_OS_WIN)
  path.append(qgetenv("HOMEPATH"));
  path.append("\\AppData\\Local\\");
  path.append(PROJECT_NAME);
  #endif

  createPath({path});
  QDir::setCurrent(path);
  createPath({"Chat"});
}

//Set application style
void setStyle(MainWindow &windows)
{
  QString mainStyle;
  QFile style(":/resources/style.css");

  if (style.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    mainStyle = style.readAll();
  }

  std::thread t([&]()
  {
    windows.setStyleSheet(mainStyle);
  });

  #if defined(Q_OS_WIN)
  for (const QString &key : QStyleFactory::keys())
  {
    if (key.toLower() == "fusion")
    {
      application.setStyle(QStyleFactory::create(key));
    }
  }
  #endif

  t.join();
}

int main(int argc, char *argv[])
{
  QApplication application(argc, argv);
  std::thread t(setFont);
  application.setApplicationVersion(APP_VERSION "-beta");
  QTranslator translator;
  setTranslation(translator);
  setPath();
  t.join();
  MainWindow windows;
  setStyle(windows);
  windows.show();
  return application.exec();
}
