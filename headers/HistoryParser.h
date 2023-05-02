#ifndef HISTORYPARSER_H
#define HISTORYPARSER_H
#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QMetaEnum>

class HistoryParser : public QObject
{
	Q_OBJECT

public:
	explicit HistoryParser(QObject *parent = nullptr, QString fileName = 0);

	enum Role
	{
		User,
		Assistant,
		System
	};
	Q_ENUM(Role)
	struct Message
	{
		Role role;
		QString content;
	};

	void addMessage(quint16 index, QString content);
	void addNewMessage(Message message);
	void editMessage(quint16 index, QString content);
	void editMessage(quint16 index, quint8 number, QString content);
	void deleteMessage(quint16 index);
	void deleteMessage(quint16 index, quint8 number);
	Message getMessage(quint16 index);
	Message getMessage(quint16 index, quint8 number);
	quint16 getCountMessage();

private:
	QFile m_history;
	quint16 m_countMessage = 0;

};

#endif // HISTORYPARSER_H
