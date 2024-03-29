#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QToolButton>
#include <thread>

#include "HistoryParser.h"
#include "ChatGPT.h"
#include "ChatSettings.h"
#include "ChatSettingsDialog.h"
#include "SettingsWidget.h"
#include "SetupDialog.h"

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
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
  void resizeEvent(QResizeEvent *event) override;

private:
  Ui::MainWindow *m_ui;
  QVector<ChatGPT*> m_chatGPTList;
  bool m_isWaitAnswer;
  bool m_isErrorAnswer;
  QMovie m_movie;
  ChatSettings m_chatSettings;
  QSoundEffect m_answerEffect;
  QSoundEffect m_errorEffect;
  QSharedPointer<ChatSettingsDialog> m_chatSettingsDialog;
  QSharedPointer<SettingsWidget> m_settingsWidget;
  QSharedPointer<SetupDialog> m_setupDialog;

  void setChatSettings(const quint8 &index);
  void receivedText(QString text);
  void fillChatList();
  bool checkExistChats();
  bool createChat(QString name);
  void showChat();
  void answerState(bool waitAnswer);
  void errorState(bool error);
  void tokensLeft();
  void setFileChatSettings(const quint8 &index);
  void sendMessage(quint16 index = 0);
  void addMessages(HistoryParser::Messages message, quint8 chatIndex);
  void addChatItem(QString name);
  ChatGPT *addChatGPT(const quint16 &messageIndex);
  void deleteChatGPT(const quint16 &chatIndex);
  quint16 getIndexChatGPT(ChatGPT *chatGPT) const;
  QVector<HistoryParser::Messages> getAllMesages();

private slots:
  void sendClicked();
  void newChatClicked();
  void chatItemChanged(QListWidgetItem *item);
  void settingsClicked();
  void stopClicked();
  void chatSettingsClicked();
  void responseReceived(const quint16 messageIndex = 0);
  void replyError(QString error);
  void chatItemDeleteClicked();
  void messageDelete();
  void scrollToBottom();
  void setAutoNameChat();

signals:
  void resized();
};
#endif // MAINWINDOW_H
