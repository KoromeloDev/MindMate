#ifndef UPDATER_H
#define UPDATER_H

#include <QWidget>
#include <QArchive>
#include <QDir>

#include "UpdateChecker.h"
#include "UpdateDownloader.h"

namespace Ui
{
  class Updater;
}

class Updater : public QWidget
{
  Q_OBJECT

public:
  explicit Updater(QWidget *parent = nullptr);
  ~Updater();

private:
  Ui::Updater *m_ui;
  QSharedPointer<UpdateChecker> m_updateChecker;
  QSharedPointer<UpdateDownloader> m_updateDownloader;
  QSharedPointer<QArchive::DiskExtractor> m_extractor;

  QString convert(quint64 bytes);

private slots:
  void replyError(QString error);
  void needUpdate(QVector<QUrl> urlList);
  void downloadCompleted(const QString &filePath, quint64 totalSize);
  void downloadFailed();
  void downloadProgress(quint64 received, quint64 total);
  void extractorError(short errorCode);
  void extractorFinished();
  void extractorProgress(QString file, int processedEntries, int totalEntries,
                         qint64 bytesProcessed, qint64 bytesTotal);

signals:
  void openMainWindow();
};

#endif // UPDATER_H
