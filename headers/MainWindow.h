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

#include "HistoryParser.h"
#include "ChatGPT.h"
#include "ChatSettings.h"
#include "MessageWidget.h"

#if CHECKUPDATES
#include "UpdateChecker.h"
#endif

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
  void paintEvent(QPaintEvent *event) override;

private:
  Ui::MainWindow *m_ui;
  QVector<ChatGPT*> m_chatGPTList;
  bool m_isWaitAnswer;
  bool m_isErrorAnswer;
  QMovie m_movie;
  QVector<HistoryParser::Message> m_allMesages;
  ChatSettings m_chatSettings;
  QSoundEffect m_answerEffect;
  QSoundEffect m_errorEffect;
  bool m_canScroll;

  #if CHECKUPDATES
  QSharedPointer<UpdateChecker> m_updateChecker;

  void needUpdates(bool haveUpdates, QUrl downloadUrl);
  #endif

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
  void sendMessage();
  void addMessage(MessageWidget *messageWidget);

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
