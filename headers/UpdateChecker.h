#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>

class UpdateChecker : public QObject
{
	Q_OBJECT

public:
	explicit UpdateChecker(QObject *parent = nullptr, QString user = 0,
												 QString repo = 0);
	~UpdateChecker();

	void checkUpdates();

private:
	QNetworkAccessManager *m_networkAccessManager;
	QString m_user;
	QString m_repo;

signals:
	void needUpdates(bool haveUpdates, QString version = 0);
};

#endif // UPDATECHECKER_H
