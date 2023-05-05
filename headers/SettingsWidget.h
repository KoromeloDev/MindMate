#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H
#pragma once

#include <QDialog>
#include <QSettings>
#include <QMessageBox>

#include "ChatSettingsWidget.h"

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

	const QString getOpenAIKey();
	const bool getCheckUpdates();
	const bool getLanguageRecognize();
	const bool getAutoNaming();
	const ChatSettings getChatSettings();

protected:
	void closeEvent(QCloseEvent *event) override;

private:
	Ui::SettingsWidget *m_ui;
	QString m_openAIKey;
	bool m_checkUpdates;
	bool m_languageRecognize;
	bool m_autoNaming;
	ChatSettingsWidget m_chatSettingsWidget;
	ChatSettings m_chatSettings;

	void writeSettings();
	void readSettings();
	void showSettings();

private slots:
	void editKeyClicked();
	void checkUpdatesStateChanged(quint8 state);
};

#endif // SETTINGSWIDGET_H
