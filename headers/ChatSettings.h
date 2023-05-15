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
  QString model = "gpt-3.5-turbo";
  quint32 usedTokens;
  quint32 maxTokens;
  float temperature = 1;
  quint8 n = 1;
  QStringList stop;
  float presencePenalty = 0;
  float frequencyPenalty = 0;

  static ChatSettings getSettings(quint8 index);

};

#endif // CHATSETTINGS_H
