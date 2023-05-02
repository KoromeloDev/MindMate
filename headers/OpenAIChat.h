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

	void chat(QList<HistoryParser::Message> message,
						ChatSettings chatSettings, quint8 index);
	void chat(QString message);
	void stopChat();
	HistoryParser::Message getAnswerMessage();
	quint32 getUsedToken();
	quint8 getIndex() const;
	bool isError();

signals:
	void responseReceived(OpenAIChat *self, QString response);
	void replyError(OpenAIChat *self, QString error);

private slots:
	void onFinished(QNetworkReply *reply);

private:
	QNetworkAccessManager *m_networkManager;
	QString m_key;
	QList<HistoryParser::Message> m_message;
	ChatSettings m_chatSettings;
	HistoryParser::Message m_answerMessage;
	quint32 m_usedToken;
	quint8 m_index;
	bool m_error;

	void send(QJsonObject json);
};

#endif // OPENAICHAT_H
