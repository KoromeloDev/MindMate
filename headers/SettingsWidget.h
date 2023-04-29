#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H
#pragma once

#include <QDialog>
#include <QSettings>
#include <QMessageBox>

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

private:
	Ui::SettingsWidget *m_ui;
	QString m_openAIKey;
	bool m_checkUpdates;

	void writeSettings();
	void readSettings();
	void showSettings();

private slots:
	void editKeyClicked();
	void checkUpdatesStateChanged(quint8 state);
};

#endif // SETTINGSWIDGET_H
