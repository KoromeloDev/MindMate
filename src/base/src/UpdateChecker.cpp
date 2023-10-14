#include "UpdateChecker.h"

UpdateChecker::UpdateChecker()
{
  m_networkManager = nullptr;
}

UpdateChecker::~UpdateChecker()
{
  if (m_networkManager != nullptr)
  {
    m_networkManager->deleteLater();
  }
}

void UpdateChecker::checkUpdate(QString owner, QString repo)
{
  if (m_networkManager != nullptr)
  {
    return;
  }

  m_timer = m_timer.create();
  m_timer->setInterval(10000);
  m_timer->setSingleShot(true);
  m_networkManager = new QNetworkAccessManager(this);
  QUrl url = QString("https://api.github.com/repos/%1/%2/releases/latest").arg(
                  owner, repo);
  QNetworkRequest request(url);

  connect(m_networkManager, &QNetworkAccessManager::finished,
          this, &UpdateChecker::onFinished);
  connect(m_timer.get(), &QTimer::timeout,
          this, &UpdateChecker::timeout);

  m_networkManager->get(request);
  m_timer->start();
}

bool UpdateChecker::compareVersion(QString tag)
{
  static QRegularExpression re("v?(0|[1-9]\\d*)\\.(0|[1-9]\\d*)\\.(0|[1-9]\\d*)"
  "(?:-((?:0|[1-9]\\d*|\\d*[a-zA-Z-][0-9a-zA-Z-]*)"
  "(?:\\.(?:0|[1-9]\\d*|\\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\\+([0-9a-zA-Z-]+"
  "(?:\\.[0-9a-zA-Z-]+)*))?");
  QRegularExpressionMatchIterator matchIterator = re.globalMatch(tag);

  if (!matchIterator.hasNext())
  {
    return false;
  }

  QString version = QApplication::applicationVersion();
  QRegularExpressionMatch matchTag = matchIterator.next();
  matchIterator = re.globalMatch(version);

  if (!matchIterator.hasNext())
  {
    return false;
  }

  QRegularExpressionMatch matchVersion = matchIterator.next();
  QString preReleaseTag = matchTag.captured(4);
  QString preReleaseVersion = matchVersion.captured(4);

  if (preReleaseTag > preReleaseVersion)
  {
    return true;
  }

  for (quint8 i = 1; i < 4; ++i)
  {
    QString stringTag = matchTag.captured(i);
    QString stringVersion = matchVersion.captured(i);

    if (stringTag > stringVersion)
    {
      return true;
    }
    else if (stringTag < stringVersion)
    {
      break;
    }
  }

  return false;
}

void UpdateChecker::onFinished(QNetworkReply *reply)
{
  m_timer.clear();
  QByteArray data = reply->readAll();
  m_networkManager->deleteLater();
  m_networkManager = nullptr;

  if (data.isEmpty())
  {
    emit replyError(tr("The data is empty"));
    return;
  }
  else if (reply->error() == QNetworkReply::NoError)
  {
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject json = document.object();
    QString tag = json.value("tag_name").toString();

    if (!compareVersion(tag))
    {
      emit replyError(tr("No need to update"));
      return;
    }

    QJsonArray assets = json.value("assets").toArray();
    QVector<QUrl> urlList;

    for (const auto &asset : assets)
    {
      urlList.append(asset.toObject().value("browser_download_url").toString());
    }

    emit needUpdate(urlList);
  }
  else
  {
    emit replyError(reply->errorString());
  }
}

void UpdateChecker::timeout()
{
  if (m_timer.isNull())
  {
    return;
  }

  emit replyError(tr("Timeout"));
}
