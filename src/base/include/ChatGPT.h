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
  explicit ChatGPT(QString key = 0);
  ~ChatGPT();

  void send(QVector<HistoryParser::Messages> message,
            ChatSettings chatSettings);
  void send(QString message, ChatSettings chatSettings);
  HistoryParser::Messages getAnswerMessage() const;
  quint32 getUsedToken() const;
  bool isError() const;

private:
  friend class ChatGPTTest;

  QNetworkAccessManager *m_networkManager;
  QString m_key;
  QVector<HistoryParser::Messages> m_message;
  ChatSettings m_chatSettings;
  HistoryParser::Messages m_answerMessage;
  quint32 m_usedToken;
  bool m_error;

  void sendJson(QJsonObject json);

signals:
  void responseReceived(QString response);
  void replyError(QString error);

private slots:
  void onFinished(QNetworkReply *reply);
};

#endif // CHATGPT_H
