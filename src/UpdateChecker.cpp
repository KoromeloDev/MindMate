#include "UpdateChecker.h"

UpdateChecker::UpdateChecker(QObject *parent, QString user, QString repo)
: QObject(parent)
{
  m_user = user;
  m_repo = repo;
}

UpdateChecker::~UpdateChecker()
{

}

void UpdateChecker::checkUpdates()
{
  if (m_networkManager != nullptr)
  {
    emit needUpdates(false);
    return;
  }

  QString apiUrl = QString(
  "https://api.github.com/repos/%1/%2/releases/latest").arg(m_user, m_repo);
  QNetworkRequest request;
  request.setUrl(QUrl(apiUrl));
  m_networkManager = m_networkManager.create(this);

  connect(m_networkManager.get(), &QNetworkAccessManager::finished,
          this, &UpdateChecker::onFinished);

  m_networkManager->get(request);
}

void UpdateChecker::onFinished(QNetworkReply *reply)
{
  QByteArray response = reply->readAll();

  disconnect(m_networkManager.get(), &QNetworkAccessManager::finished,
          this, &UpdateChecker::onFinished);

  if (reply->error() == QNetworkReply::NoError)
  {
    QJsonDocument jsonDocument = QJsonDocument::fromJson(response);
    QJsonObject object = jsonDocument.object();
    QJsonArray assets = object["assets"].toArray();
    QUrl downloadUrl;

    for (quint8 i = 0; i < assets.count(); ++i)
    {
      QString value = assets.at(i).toObject()["browser_download_url"]
                      .toString().toLower();
      QString platform;

      #if defined(Q_OS_WIN)
      platform = "windows";
      #elif defined(Q_OS_LINUX)
      platform = "linux";
      #endif

      if (value.contains(platform))
      {
        downloadUrl = value;
        break;
      }
    }

    QString tagName = object["tag_name"].toString();

    if (!downloadUrl.isEmpty() && tagName != "v" +
        QApplication::applicationVersion())
    {
      emit needUpdates(true, downloadUrl);
      return;
    }

    emit needUpdates(false);
  }
}
