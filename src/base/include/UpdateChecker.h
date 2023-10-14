#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>
#include <QTimer>

class UpdateChecker : public QObject
{
  Q_OBJECT
public:
  UpdateChecker();
  ~UpdateChecker();

  void checkUpdate(QString owner, QString repo);

private:
  QNetworkAccessManager *m_networkManager;
  QSharedPointer<QTimer> m_timer;

  bool compareVersion(QString tag);

private slots:
  void onFinished(QNetworkReply *reply);
  void timeout();

signals:
  void replyError(QString error);
  void needUpdate(QVector<QUrl> urlList);
};

#endif // UPDATECHECKER_H
