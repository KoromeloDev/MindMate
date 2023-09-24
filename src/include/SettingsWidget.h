#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QDialog>
#include <QSettings>
#include <QClipboard>
#include <QTimer>

#include "ChatSettingsWidget.h"
#include "Settings.h"
#include "APIKeyWidget.h"

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
  QSharedPointer<APIKeyWidget> m_keyWidget;
  QSharedPointer<ChatSettingsWidget> m_chatSettingsWidget;
  Settings m_settings;
  QClipboard *m_clipboard;
  QSharedPointer<QTimer> m_timer;

private slots:
  void languageRecognizeStateChanged(quint8 state);
  void autoNamingStateChanged(quint8 state);
  void copyClicked();
  void timerTimeout();
  void homepageClicked();
  void reportClicked();
};

#endif // SETTINGSWIDGET_H
