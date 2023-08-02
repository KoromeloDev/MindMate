#include "ChatSettings.h"

ChatSettings::ChatSettings()
{
  model = "gpt-3.5-turbo";
  temperature = 1;
  n = 1;
  presencePenalty = 0;
  frequencyPenalty = 0;
}

ChatSettings ChatSettings::getSettings(quint8 index) const
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
    chatSettings.temperature = object["temperature"].toDouble();
    chatSettings.n = object["n"].toInt();
    QJsonArray stops = object["stop"].toArray();

    for (const auto &stop : stops)
    {
      chatSettings.stop.append(stop.toString());
    }

    chatSettings.presencePenalty = object["presence_penalty"].toDouble();
    chatSettings.frequencyPenalty = object["frequency_penalty"].toDouble();
  }

  return chatSettings;
}

quint32 ChatSettings::getMaxTokens(QString model) const
{
  if (model == "gpt-3.5-turbo-0301" || model == "gpt-3.5-turbo")
  {
    return 4096;
  }
  else if (model == "gpt-4" || model == "gpt-4-0314")
  {
    return 16384;
  }
  else if (model == "gpt-4-32k-0314" || model == "gpt-4")
  {
    return 32768;
  }

  return 0;
}

quint32 ChatSettings::getMaxTokens() const
{
  return getMaxTokens(model);
}
