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
  QString fontFileName = ":/fonts/Roboto-Regular.ttf";
  QFontDatabase::addApplicationFont(fontFileName);
  QApplication::setFont(QFont("Roboto", 11));
}

//Translation of the application
void setTranslation(QTranslator &translator)
{
  const QVector<QString> uiLanguages = QLocale::system().uiLanguages();

  for (const QString &locale : uiLanguages)
  {
    if (translator.load(":/i18n/" + QLocale(locale).name().toLower()))
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
  path.append(qgetenv("LOCALAPPDATA"));

  if (!path.contains(PROJECT_NAME))
  {
    path.append("\\");
    path.append(PROJECT_NAME);
  }
  #endif

  if(!QDir(path).exists())
  {
    QDir(path).mkdir(path);
  }

  QDir::setCurrent(path);
  createPath({"Chat"});
}

//Set application style
void setStyle(MainWindow &windows)
{
  QString mainStyle;
  QFile style(":/style.css");

  if (style.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    mainStyle = style.readAll();
    windows.setStyleSheet(mainStyle);
  }
}

int main(int argc, char *argv[])
{
  QApplication application(argc, argv);
  std::thread tFont(setFont);
  application.setApplicationVersion(APP_VERSION "-beta");
  QTranslator translator;
  std::thread tTranslator(setTranslation, std::ref(translator));
  setPath();
  tTranslator.join();
  tFont.join();

  //Set application style for windows
  #if defined(Q_OS_WIN)
  for (const QString &key : QStyleFactory::keys())
  {
    if (key.toLower() == "fusion")
    {
      QApplication::setStyle(QStyleFactory::create(key));
    }
  }
  #endif

  MainWindow windows;
  setStyle(windows);
  windows.show();
  return application.exec();
}
