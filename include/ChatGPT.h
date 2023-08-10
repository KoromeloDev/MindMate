#ifndef CHATGPT_H
#define CHATGPT_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "HistoryParser.h"
#include "ChatSettings.h"

class ChatGPT : public QObject
{
  Q_OBJECT

public:
  explicit ChatGPT(QObject *parent = nullptr, QString key = 0);
  ~ChatGPT();
  void send(QVector<HistoryParser::Message> message,
            ChatSettings chatSettings, quint8 index);
  void send(QString message, ChatSettings chatSettings);
  HistoryParser::Message getAnswerMessage() const;
  quint32 getUsedToken() const;
  quint8 getIndex() const;
  bool isError() const;


private:
  QSharedPointer<QNetworkAccessManager> m_networkManager;
  QString m_key;
  QVector<HistoryParser::Message> m_message;
  ChatSettings m_chatSettings;
  HistoryParser::Message m_answerMessage;
  quint32 m_usedToken;
  quint8 m_index;
  bool m_error;

  void sendJson(QJsonObject json);

signals:
  void responseReceived(QString response);
  void replyError(QString error);

private slots:
  void onFinished(QNetworkReply *reply);

};

#endif // CHATGPT_H
