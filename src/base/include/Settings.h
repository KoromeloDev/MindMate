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

  void readSettings();
  void writeSettings();

};

#endif // SETTINGS_H
