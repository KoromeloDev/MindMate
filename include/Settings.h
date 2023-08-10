#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

#include "ChatSettings.h"

struct Settings
{
  Settings();

  #if CHECKUPDATES
  bool checkUpdates;
  #endif

  bool languageRecognize;
  bool autoNaming;
  QString openAIKey;
  ChatSettings chatSettings;

  void readSettings();
  void writeSettings();

};

#endif // SETTINGS_H
