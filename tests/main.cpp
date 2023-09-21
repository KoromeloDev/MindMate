#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  QString path = QCoreApplication::applicationDirPath();
  QString searchPattern = "*Test.exe";
  QDir dir(path);
  QStringList fileList = dir.entryList(QStringList() <<
                                       searchPattern, QDir::Files);

  if (fileList.isEmpty())
  {
    qDebug() << "Nothing to run";
    return 0;
  }

  bool hasError = false;

  for (const QString &fileName : fileList)
  {
    QString filePath = path + QDir::separator() + fileName;
    qDebug() << "Starting:" << fileName;
    QProcess process;
    process.start(filePath);

    if (process.waitForFinished(-1))
    {
      QByteArray output = process.readAllStandardOutput();

      if (output.isEmpty())
      {
        qDebug() << "Empty output";
        qDebug() << fileName << "failed" << Qt::endl;
        hasError = true;
      }
      if (process.exitCode() == 0)
      {
        qDebug().noquote() << output;
        qDebug() << fileName << "successfully" << Qt::endl;
      }
      else
      {
        qDebug().noquote() << output;
        qDebug() << fileName << "failed" << Qt::endl;
        hasError = true;
      }
    }
    else
    {
      qDebug() << "Failed to run the test" << fileName;
      hasError = true;
    }
  }

  if (hasError)
  {
    return 1;
  }

  return 0;
}
