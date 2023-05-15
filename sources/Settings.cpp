#include "Settings.h"

Settings::Settings()
{
  readSettings();
}

void Settings::readSettings()
{
  QSettings settings("settings.ini", QSettings::IniFormat);
  settings.beginGroup("Main");

  #if CHECKUPDATES
  checkUpdates = settings.value("CheckUpdates").toBool();
  #endif

  if (settings.value("AutoLanguageRecognize").isValid())
  {
    languageRecognize = settings.value("AutoLanguageRecognize").toBool();
  }

  if (settings.value("AutoNamingChat").isValid())
  {
    autoNaming = settings.value("AutoNamingChat").toBool();
  }

  settings.endGroup();

  settings.beginGroup("Chat");
  openAIKey = settings.value("OpenAIKey").toString();

  QString model = settings.value("Model").toString();

  if (!model.isEmpty())
  {
    chatSettings.model = model;
  }

  if (!settings.value("Temperature").isNull())
  {
    chatSettings.temperature = settings.value("Temperature").toFloat();
  }

  if (!settings.value("N").isNull())
  {
    chatSettings.n = settings.value("N").toUInt();
  }

  QStringList stopList;

  for (quint8 i = 0; i < 4; i++)
  {
    QString stopWord = settings.value("StopWord" + QString::number(i)).
                       toString();
    if (!stopWord.isEmpty())
    {
      stopList.append(stopWord);
    }
  }

  if (stopList.length() !=0)
  {
    chatSettings.stop = stopList;
  }

  if (!settings.value("PresencePenalty").isNull())
  {
    chatSettings.presencePenalty = settings.value("PresencePenalty").toFloat();
  }

  if (!settings.value("FrequencyPenalty").isNull())
  {
    chatSettings.frequencyPenalty = settings.value("FrequencyPenalty").
                                    toFloat();
  }

  settings.endGroup();
}

void Settings::writeSettings()
{
  QSettings settings("settings.ini", QSettings::IniFormat);
  settings.beginGroup("Main");

  #if CHECKUPDATES
  settings.setValue("CheckUpdates", checkUpdates);
  #endif

  settings.setValue("AutoLanguageRecognize", languageRecognize);
  settings.setValue("AutoNamingChat", autoNaming);
  settings.endGroup();

  settings.beginGroup("Chat");
  settings.setValue("OpenAIKey", openAIKey);
  settings.setValue("Model", chatSettings.model);
  settings.setValue("Temperature", chatSettings.temperature);
  settings.setValue("N", chatSettings.n);

  for (quint8 i = 0; i < chatSettings.stop.length(); i++)
  {
    settings.setValue("StopWord" + QString::number(i), chatSettings.stop[i]);
  }

  settings.setValue("PresencePenalty", chatSettings.presencePenalty);
  settings.setValue("FrequencyPenalty", chatSettings.frequencyPenalty);
  settings.endGroup();
}
