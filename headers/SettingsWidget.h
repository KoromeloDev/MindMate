#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H
#pragma once

#include <QDialog>
#include <QSettings>
#include <QMessageBox>

#include "ChatSettingsWidget.h"
#include "Settings.h"

namespace Ui
{
  class SettingsWidget;
}

class SettingsWidget : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsWidget(QWidget *parent = nullptr);
  ~SettingsWidget();

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  Ui::SettingsWidget *m_ui;
  ChatSettingsWidget *m_chatSettingsWidget;
  Settings m_settings;

private slots:
  void editKeyClicked();

  #if CHECKUPDATES
  void checkUpdatesStateChanged(quint8 state);
  #endif

  void languageRecognizeStateChanged(quint8 state);
  void autoNamingStateChanged(quint8 state);

};

#endif // SETTINGSWIDGET_H
