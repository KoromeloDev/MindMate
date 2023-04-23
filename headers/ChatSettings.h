#ifndef CHATSETTINGS_H
#define CHATSETTINGS_H
#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>

struct ChatSettings
{
	QString name;
	QString fileName;
	QString model;
	quint32 usedTokens;
	quint32 maxTokens;
	float temperature;
	quint8 n;
	QStringList stop;
	float presencePenalty;
	float frequencyPenalty;

	static ChatSettings getSettings(quint8 index);

};

#endif // CHATSETTINGS_H
