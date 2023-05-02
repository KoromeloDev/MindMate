#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QListWidget>
#include <QMovie>
#include <QUuid>
#include <QScrollBar>
#include <QSoundEffect>

#include "HistoryParser.h"
#include "OpenAIChat.h"
#include "ChatItem.h"
#include "ChatSettings.h"
#include "MessageWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

protected:
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;

private:
	Ui::MainWindow *m_ui;
	QList<OpenAIChat *> m_chatGPT;
	bool m_waitAnswer = false;
	bool m_errorAnswer = false;
	QMovie m_movie;
	QList<HistoryParser::Message> m_allMesages;
	ChatSettings m_chatSettings;
	QSoundEffect *m_answerEffect;
	QSoundEffect *m_errorEffect;
	bool m_scroll = false;

	void setChatSettings(quint8 index);
	void receivedText(QString text);
	void fillChatList();
	bool checkExistChats();
	bool createChat(QString name);
	void showChat();
	void answerState(bool waitAnswer);
	void errorState(bool error);
	void tokensLeft();
	void setFileChatSettings(quint8 index);
	void sendMessage();

private slots:
	void sendClicked();
	void newChatClicked();
	void chatItemChanged(QListWidgetItem *item);
	void settingsClicked();
	void retryClicked();
	void stopClicked();
	void chatSettingsClicked();
	void responseReceived(QString response);
	void replyError(QString error);
	void chatItemDeleteClicked();
	void messageDeleteCliked();
};
#endif // MAINWINDOW_H
