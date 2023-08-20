#ifndef CHATSETTINGS_H
#define CHATSETTINGS_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>

class ChatSettings
{
public:
  ChatSettings();

  QString name;
  QString fileName;
  QString model;
  quint32 usedTokens;
  float temperature;
  quint8 n;
  QVector<QString> stop;
  float presencePenalty;
  float frequencyPenalty;

  ChatSettings getSettings(quint8 index) const;
  quint32 getMaxTokens(QString model) const;
  quint32 getMaxTokens() const;

private:
  quint32 m_maxTokens;

};

#endif // CHATSETTINGS_H
