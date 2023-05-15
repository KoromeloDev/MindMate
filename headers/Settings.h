#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

#include "ChatSettings.h"

struct Settings
{
  Settings();

  #if CHECKUPDATES
  bool checkUpdates = false;
  #endif

  bool languageRecognize = true;
  bool autoNaming = true;
  QString openAIKey;
  ChatSettings chatSettings;

  void readSettings();
  void writeSettings();

};

#endif // SETTINGS_H
