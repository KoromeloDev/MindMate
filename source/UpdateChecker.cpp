#include "UpdateChecker.h"

UpdateChecker::UpdateChecker(QObject *parent, QString user, QString repo)
: QObject(parent), m_user(user), m_repo(repo)
{

}

UpdateChecker::~UpdateChecker()
{

}

void UpdateChecker::checkUpdates()
{
	const QString apiUrl = QString(
	"https://api.github.com/repos/%1/%2/releases/latest").arg(m_user, m_repo);
	QNetworkRequest request;
	request.setUrl(QUrl(apiUrl));
	m_networkAccessManager = new QNetworkAccessManager(this);
	QNetworkReply* reply = m_networkAccessManager->get(request);

	connect(reply, &QNetworkReply::finished, this, [=]()
	{
		const QByteArray response = reply->readAll();
		m_networkAccessManager->deleteLater();

		if (reply->error() == QNetworkReply::NoError)
		{
			const QJsonDocument jsonDocument = QJsonDocument::fromJson(response);
			const QString tagName = jsonDocument.object()["tag_name"].toString();

            if (tagName != "v" + QApplication::applicationVersion())
			{
				emit needUpdates(true, tagName);
				return;
			}

			emit needUpdates(false);
		}
	});
}
