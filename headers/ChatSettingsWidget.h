#ifndef CHATSETTINGSWIDGET_H
#define CHATSETTINGSWIDGET_H
#pragma once

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "ChatSettings.h"

namespace Ui
{
class ChatSettingsWidget;
}

class ChatSettingsWidget : public QDialog
{
	Q_OBJECT

public:
	explicit ChatSettingsWidget(QWidget *parent = nullptr, quint8 index = 0);
	~ChatSettingsWidget();

private:
	Ui::ChatSettingsWidget *m_ui;
	ChatSettings m_chatSettings;
	quint8 m_index;

	void setParameters();

public slots:
	void accept() override;

private slots:
	void modelChanged(QString text);
	void temperatureSliderMoved(quint8 position);
	void nSliderMoved(quint8 position);
	void pPSliderMoved(qint8 position);
	void fPSliderMoved(qint8 position);
};

#endif // CHATSETTINGSWIDGET_H
