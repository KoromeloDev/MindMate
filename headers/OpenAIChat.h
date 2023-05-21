#ifndef OPENAICHAT_H
#define OPENAICHAT_H
#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "HistoryParser.h"
#include "ChatSettings.h"

class OpenAIChat : public QObject
{
  Q_OBJECT

public:
  OpenAIChat(QObject *parent = nullptr, QString key = 0);
  ~OpenAIChat();

  void send(QList<HistoryParser::Message> message,
            ChatSettings chatSettings, quint8 index);
  void send(QString message, ChatSettings chatSettings = {});
  void stopChat();
  HistoryParser::Message getAnswerMessage() const;
  quint32 getUsedToken() const;
  quint8 getIndex() const;
  bool isError() const;

private:
  QNetworkAccessManager *m_networkManager = nullptr;
  QString m_key;
  QList<HistoryParser::Message> m_message;
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

#endif // OPENAICHAT_H
