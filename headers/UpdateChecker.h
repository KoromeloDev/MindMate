#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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
  QNetworkAccessManager *m_networkAccessManager = nullptr;
  QString m_user;
  QString m_repo;

  void onFinished(QNetworkReply* reply);

signals:
  void needUpdates(bool haveUpdates, QUrl downloadUrl = {});
};

#endif // UPDATECHECKER_H
