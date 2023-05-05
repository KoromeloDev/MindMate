#ifndef CHATSETTINGSDIALOG_H
#define CHATSETTINGSDIALOG_H
#pragma once

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "ChatSettingsWidget.h"

namespace Ui
{
	class ChatSettingsDialog;
}

class ChatSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ChatSettingsDialog(QWidget *parent = nullptr, quint8 index = 0);
	~ChatSettingsDialog();

private:
	Ui::ChatSettingsDialog *m_ui;
	quint8 m_index;
	ChatSettingsWidget m_settingsWidget;

public slots:
	void accept() override;

};

#endif // CHATSETTINGSDIALOG_H
