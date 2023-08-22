#include "Settings.h"

Settings::Settings()
{
  languageRecognize = true;
  autoNaming = true;

  #if CHECKUPDATES
  checkUpdates = false;
  #endif

  readSettings();
}

void Settings::readSettings()
{
  QSettings settings("settings.ini", QSettings::IniFormat);
  settings.beginGroup("Main");
  QVariant v_languageRecognize = settings.value("AutoLanguageRecognize");
  QVariant v_autoNaming = settings.value("AutoNamingChat");

  #if CHECKUPDATES
  checkUpdates = settings.value("CheckUpdates").toBool();
  #endif

  if (v_languageRecognize.isValid())
  {
    languageRecognize = v_languageRecognize.toBool();
  }

  if (v_autoNaming.isValid())
  {
    autoNaming = v_autoNaming.toBool();
  }

  settings.endGroup();

  settings.beginGroup("Chat");
  openAIKey = settings.value("OpenAIKey").toString();
  QVariant v_temperature = settings.value("Temperature");
  QVariant v_n = settings.value("N");
  QVariant v_presencePenalty = settings.value("PresencePenalty");
  QVariant v_frequencyPenalty = settings.value("FrequencyPenalty");

  QString model = settings.value("Model").toString();

  if (!model.isEmpty())
  {
    chatSettings.model = model;
  }

  if (!v_temperature.isNull())
  {
    chatSettings.temperature = v_temperature.toFloat();
  }

  if (!v_n.isNull())
  {
    chatSettings.n = v_n.toUInt();
  }

  QVector<QString> stopList;

  for (quint8 i = 0; i < 4; ++i)
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

  if (!v_presencePenalty.isNull())
  {
    chatSettings.presencePenalty = v_presencePenalty.toFloat();
  }

  if (!v_frequencyPenalty.isNull())
  {
    chatSettings.frequencyPenalty = v_frequencyPenalty.toFloat();
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

  for (quint8 i = 0; i < 4; ++i)
  {
    QString stop = chatSettings.stop.length() > i ? chatSettings.stop[i] : "";
    settings.setValue("StopWord" + QString::number(i), stop);
  }

  settings.setValue("PresencePenalty", chatSettings.presencePenalty);
  settings.setValue("FrequencyPenalty", chatSettings.frequencyPenalty);
  settings.endGroup();
}
