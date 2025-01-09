#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

#include "ChatSettings.h"

struct Settings
{
  Settings();

  bool languageRecognize;
  bool autoNaming;
  QString openAIKey;
  ChatSettings chatSettings;
  QString userMessageColor1;
  QString userMessageColor2;
  QString assistantMessageColor1;
  QString assistantMessageColor2;
  QString systemMessageColor1;
  QString systemMessageColor2;

  void readSettings();
  void writeSettings();
};

#endif // SETTINGS_H
