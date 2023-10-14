#include "UpdateDownloader.h"

UpdateDownloader::UpdateDownloader(QObject *parent) : QObject{parent}
{
  m_networkManager = nullptr;
  m_currentReply = nullptr;
}

UpdateDownloader::~UpdateDownloader()
{
  if (m_networkManager != nullptr)
  {
    m_networkManager->deleteLater();
  }

  if (m_currentReply != nullptr)
  {
    m_currentReply->deleteLater();
  }
}

void UpdateDownloader::download(const QUrl &url)
{
  if (m_networkManager != nullptr || url.isEmpty())
  {
    emit downloadFailed();
    return;
  }

  m_file = m_file.create(m_tempDir.path() + "/" + url.fileName());

  if (!m_file->open(QIODevice::WriteOnly))
  {
    m_file.clear();
    emit downloadFailed();
    return;
  }

  m_networkManager = new QNetworkAccessManager(this);
  QNetworkRequest request(url);

  connect(m_networkManager, &QNetworkAccessManager::finished,
          this, &UpdateDownloader::onReplyFinished);

  m_currentReply = m_networkManager->get(request);

  connect(m_currentReply, &QNetworkReply::readyRead,
          this, &UpdateDownloader::onReadyRead);
  connect(m_currentReply, &QNetworkReply::downloadProgress,
          this, &UpdateDownloader::onDownloadProgress);
}

void UpdateDownloader::onDownloadProgress(quint64 received, quint64 total)
{
  emit downloadProgress(received, total);
}

void UpdateDownloader::onReplyFinished(QNetworkReply *reply)
{
  m_networkManager->deleteLater();
  m_networkManager = nullptr;

  if (reply->error() == QNetworkReply::NoError)
  {
    qint64 totalSize = reply->header(QNetworkRequest::ContentLengthHeader).
                       toLongLong();
    QString filePath = m_file->fileName();
    emit downloadCompleted(filePath, totalSize);
  }
  else
  {
    emit downloadFailed();
  }

  m_file->close();
  m_currentReply->deleteLater();
  m_currentReply = nullptr;
}

void UpdateDownloader::onReadyRead()
{
  if (m_file.get())
  {
    m_file->write(m_currentReply->readAll());
  }
}
