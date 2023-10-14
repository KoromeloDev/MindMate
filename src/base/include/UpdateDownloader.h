#ifndef UPDATEDOWNLOADER_H
#define UPDATEDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QTemporaryDir>

class UpdateDownloader : public QObject
{
  Q_OBJECT
public:
  explicit UpdateDownloader(QObject *parent = nullptr);
  ~UpdateDownloader();

  void download(const QUrl &url);

private:
  QNetworkAccessManager* m_networkManager;
  QNetworkReply *m_currentReply;
  QSharedPointer<QFile> m_file;
  QTemporaryDir m_tempDir;

private slots:
  void onDownloadProgress(quint64 received, quint64 total);
  void onReplyFinished(QNetworkReply *reply);
  void onReadyRead();

signals:
  void downloadCompleted(const QString &filePath, quint64 totalSize);
  void downloadFailed();
  void downloadProgress(quint64 received, quint64 total);
};

#endif // UPDATEDOWNLOADER_H
