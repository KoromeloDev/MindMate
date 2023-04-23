#include "ChatSettings.h"

ChatSettings ChatSettings::getSettings(quint8 index)
{
	QFile chatJson(QDir::currentPath() + "/chats.json");
	ChatSettings chatSettings;

	if (chatJson.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QByteArray jsonData = chatJson.readAll();
		chatJson.close();
		QJsonDocument document = QJsonDocument::fromJson(jsonData);
		QJsonArray chats = document.array();

		if (chats.isEmpty())
		{
			return chatSettings;
		}

		QJsonObject object = chats[index].toObject();
		chatSettings.name = object["name"].toString();
		chatSettings.fileName = object["file_name"].toString();
		chatSettings.model = object["model"].toString();
		chatSettings.usedTokens = object["used_tokens"].toInt();
		chatSettings.maxTokens = object["max_tokens"].toInt();
		chatSettings.temperature = object["temperature"].toDouble();
		chatSettings.n = object["n"].toInt();
		QJsonArray stop = object["stop"].toArray();

		for (quint8 i = 0; i < stop.count(); i++)
		{
			chatSettings.stop.append(stop[i].toString());
		}

		chatSettings.presencePenalty = object["presence_penalty"].toDouble();
		chatSettings.frequencyPenalty = object["frequency_penalty"].toDouble();
	}

	return chatSettings;
}
