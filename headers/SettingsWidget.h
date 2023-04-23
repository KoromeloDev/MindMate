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

	QString getOpenAIKey();

private:
	Ui::SettingsWidget *m_ui;
	QString m_openAIKey;

	void writeSettings();
	void readSettings();
	void showSettings();

private slots:
	void editKeyClicked();
};

#endif // SETTINGSWIDGET_H
