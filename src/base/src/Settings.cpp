#include "Settings.h"

Settings::Settings()
{
  languageRecognize = true;
  autoNaming = true;
  readSettings();
}

void Settings::readSettings()
{
  QSettings settings("settings.ini", QSettings::IniFormat);
  settings.beginGroup("Main");
  QVariant v_languageRecognize = settings.value("AutoLanguageRecognize");
  QVariant v_autoNaming = settings.value("AutoNamingChat");

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

  if (!stopList.isEmpty())
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

  settings.beginGroup("Theme");
  userMessageColor1 = settings.value("UserMessageColor1").toString();
  userMessageColor2 = settings.value("UserMessageColor2").toString();
  assistantMessageColor1 = settings.value("AssistantMessageColor1").toString();
  assistantMessageColor2 = settings.value("AssistantMessageColor2").toString();
  systemMessageColor1 = settings.value("SystemMessageColor1").toString();
  systemMessageColor2 = settings.value("SystemMessageColor2").toString();

  if (userMessageColor1.isEmpty())
  {
    userMessageColor1 = "#40c9ff";
  }

  if (userMessageColor2.isEmpty())
  {
    userMessageColor2 = "#e81cff";
  }

  if (assistantMessageColor1.isEmpty())
  {
    assistantMessageColor1 = "#ef745c";
  }

  if (assistantMessageColor2.isEmpty())
  {
    assistantMessageColor2 = "#6e3ad5";
  }

  if (systemMessageColor1.isEmpty())
  {
    systemMessageColor1 = "#e20b8c";
  }

  if (systemMessageColor2.isEmpty())
  {
    systemMessageColor2 = "#f84b00";
  }

  settings.endGroup();
}

void Settings::writeSettings()
{
  QSettings settings("settings.ini", QSettings::IniFormat);
  settings.beginGroup("Main");
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

  settings.beginGroup("Theme");
  settings.setValue("UserMessageColor1", userMessageColor1);
  settings.setValue("UserMessageColor2", userMessageColor2);
  settings.setValue("AssistantMessageColor1", assistantMessageColor1);
  settings.setValue("AssistantMessageColor2", assistantMessageColor2);
  settings.setValue("SystemMessageColor1", systemMessageColor1);
  settings.setValue("SystemMessageColor2", systemMessageColor2);
  settings.endGroup();
}
