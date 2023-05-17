#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "NewTextEdit.h"
#include "SettingsWidget.h"
#include "ThemeIcon.h"
#include "ChatSettingsDialog.h"
#include "MessageWidget.h"
#include "ChatItem.h"
#include "Settings.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);
  Settings settings;

  connect(m_ui->textInput, &NewTextEdit::sendText,
          this, &MainWindow::receivedText);
  connect(m_ui->sendButton, &QToolButton::clicked,
          this, &MainWindow::sendClicked);
  connect(m_ui->newChatButton, &QToolButton::clicked,
          this, &MainWindow::newChatClicked);
  connect(m_ui->chatList, &QListWidget::currentItemChanged,
          this, &MainWindow::chatItemChanged);
  connect(m_ui->settingsButton, &QToolButton::clicked,
          this, &MainWindow::settingsClicked);
  connect(m_ui->retryButton, &QToolButton::clicked,
          this, &MainWindow::retryClicked);
  connect(m_ui->stopButton, &QToolButton::clicked,
          this, &MainWindow::stopClicked);
  connect(m_ui->chatSettingsButton, &QToolButton::clicked,
          this, &MainWindow::chatSettingsClicked);

  #if CHECKUPDATES
  if(settings.checkUpdates)
  {
    m_updateChecker = new UpdateChecker(this, "KoromeloDev", "MindMate");

    connect(m_updateChecker, &UpdateChecker::needUpdates,
            this, &MainWindow::needUpdates);

    m_updateChecker->checkUpdates();
  }
  #endif

  m_movie.setFileName(":/icons/eclipse.gif");
  m_movie.setScaledSize(QSize(32, 32));
  m_ui->loading->setMovie(&m_movie);
  setChatSettings(0);
  fillChatList();
  m_ui->historyList->verticalScrollBar()->setSingleStep(20);

  ThemeIcon::setIcon(*m_ui->newChatButton, ":/icons/add.svg");
  ThemeIcon::setIcon(*m_ui->settingsButton, ":/icons/settings.svg");
  ThemeIcon::setIcon(*m_ui->retryButton, ":/icons/refresh.svg");
  ThemeIcon::setIcon(*m_ui->stopButton, ":/icons/stop.svg");
  ThemeIcon::setIcon(*m_ui->chatSettingsButton, ":/icons/configuration.svg");
  ThemeIcon::setIcon(*m_ui->sendButton, ":/icons/send.svg");

  m_answerEffect.setSource(QUrl::fromLocalFile(":/sounds/message.wav"));
  m_answerEffect.setVolume(0.20f);
  m_errorEffect.setSource(QUrl::fromLocalFile(":/sounds/error.wav"));
  m_errorEffect.setVolume(0.15f);
}

MainWindow::~MainWindow()
{
  delete m_ui;

  for (OpenAIChat *chatGPT : m_chatGPTList)
  {
    if (chatGPT != nullptr)
    {
      chatGPT->deleteLater();
    }
  }

  #if CHECKUPDATES
  if (m_updateChecker != nullptr)
  {
    m_updateChecker->deleteLater();
  }
  #endif
}

#if CHECKUPDATES
void MainWindow::needUpdates(bool haveUpdates, QUrl downloadUrl)
{
  m_updateChecker->deleteLater();
  m_updateChecker = nullptr;

  if (haveUpdates)
  {
    QMessageBox::information(this, tr("Update available"),
                             "<a href='" + downloadUrl.toString() + "'>" +
                             tr("Link to download the application") +
                             "</a>");
  }
}
#endif

void MainWindow::resizeEvent(QResizeEvent *event)
{
  QMainWindow::resizeEvent(event);

  for (quint16 i = 0; i < m_ui->historyList->count(); i++)
  {
    QListWidgetItem *item = m_ui->historyList->item(i);
    QWidget *itemWidget = m_ui->historyList->itemWidget(item);
    MessageWidget *message = dynamic_cast<MessageWidget *>(itemWidget);
    message->resize();
  }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
  QMainWindow::paintEvent(event);

  if (!m_scroll)
  {
    m_ui->historyList->scrollToBottom();
    m_scroll = true;
  }
}

void MainWindow::setChatSettings(quint8 index)
{
  m_chatSettings = ChatSettings::getSettings(index);
}

void MainWindow::receivedText(QString text)
{
  if (!text.isEmpty() && ((!m_waitAnswer && !m_errorAnswer) ||
      (m_allMesages.count() != 0 &&
      m_ui->roleBox->currentIndex() != 0)))
  {
    m_ui->textInput->clear();
    HistoryParser history(this, m_chatSettings.fileName);
    HistoryParser::Message message;
    message.content = text;

    switch (m_ui->roleBox->currentIndex())
    {
      case 0:
        message.role = HistoryParser::User;
        break;
      case 1:
        message.role = HistoryParser::System;
        break;
      case 2:
        message.role = HistoryParser::Assistant;
        break;
      default:
        message.role = HistoryParser::User;
        break;
    }

    history.addNewMessage(message);
    m_allMesages.append(message);
    QListWidgetItem *item = new QListWidgetItem(m_ui->historyList);
    MessageWidget *messageWidget = new MessageWidget(item, message,
                                   m_ui->chatList->currentRow(),
                                   m_ui->historyList->count()-1);

    connect(messageWidget, &MessageWidget::selfDelete,
            this, &MainWindow::messageDeleteCliked);

    m_ui->historyList->setItemWidget(item, messageWidget);
    m_ui->historyList->setCurrentItem(item);

    if (message.role ==  HistoryParser::User)
    {
      Settings settings;

      if (m_ui->historyList->count() == 1 && settings.autoNaming)
      {
        OpenAIChat *chatGPT = new OpenAIChat(this, settings.openAIKey);

        connect(chatGPT, &OpenAIChat::responseReceived, this, [=]()
        {
          chatGPT->deleteLater();
          QString name = chatGPT->getAnswerMessage().content;

          if (name.length() < 50)
          {
            QListWidgetItem *item = m_ui->chatList->currentItem();
            QWidget *itemWidget = m_ui->chatList->itemWidget(item);
            ChatItem *chatItem = dynamic_cast<ChatItem *>(itemWidget);
            chatItem->editName(name);
          }

          sendMessage();
        });
        connect(chatGPT, &OpenAIChat::replyError, this, [=]()
        {
          chatGPT->deleteLater();
          sendMessage();
        });

        chatGPT->send("\"" + message.content + "\"" +
        tr(" - this is the first sentence in the chat, you should understand "
           "what it's about and name the chat according to its topic. "
           "Name it as briefly as possible, but keep the meaning, and try to "
           "use signs only where it is really necessary. Also, you should "
           "not name the chat like \"chat name:\", you should just write the "
           "name without unnecessary words."));
      }
      else
      {
        sendMessage();
      }
    }
    else
    {
      errorState(false);
      answerState(false);
    }
  }
}

void MainWindow::fillChatList()
{
  if (checkExistChats())
  {
    QFile chatJson(QDir::currentPath() + "/chats.json");

    if (chatJson.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      m_ui->chatList->clear();
      QByteArray jsonData = chatJson.readAll();
      chatJson.close();
      QJsonDocument document = QJsonDocument::fromJson(jsonData);
      QJsonArray chats = document.array();

      for (quint8 i = 0; i < chats.count(); i++)
      {
        QJsonObject object = chats[i].toObject();
        QString name = object["name"].toString();
        ChatItem *chatItem = new ChatItem(m_ui->chatList, name, i);
        QListWidgetItem *item = new QListWidgetItem(m_ui->chatList);

        connect(chatItem, &ChatItem::removed,
                this, &MainWindow::chatItemDeleteClicked);

        item->setSizeHint(QSize(0, 50));
        m_ui->chatList->setItemWidget(item, chatItem);
      }

      m_ui->chatList->setCurrentItem(m_ui->chatList->item(0));
    }
  }
}

bool MainWindow::checkExistChats()
{
  QFile chatJson(QDir::currentPath() + "/chats.json");

  if (chatJson.exists())
  {
    if (chatJson.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QByteArray jsonData = chatJson.readAll();
      chatJson.close();
      QJsonDocument document = QJsonDocument::fromJson(jsonData);
      QJsonArray chats = document.array();

      if (!chats.empty())
      {
        return true;
      }
    }
  }

  return createChat(tr("New chat"));
}

bool MainWindow::createChat(QString name)
{
  Settings settingsWidget;
  ChatSettings chatSettings = settingsWidget.chatSettings;
  QFile chatJson(QDir::currentPath() + "/chats.json");
  QJsonObject object;
  object.insert("name", name);
  QString uniqueFileName = QUuid::createUuid().
                           toString(QUuid::WithoutBraces).left(4);
  object.insert("file_name", uniqueFileName);
  object.insert("model", chatSettings.model);
  object.insert("used_tokens", 0);
  object.insert("max_tokens", 4096);
  object.insert("temperature", chatSettings.temperature);
  object.insert("n", chatSettings.n);
  QJsonArray stop;

  for (const QString &stopWord : chatSettings.stop)
  {
    stop.append(stopWord);
  }

  object.insert("stop", stop);
  object.insert("presence_penalty", chatSettings.presencePenalty);
  object.insert("frequency_penalty", chatSettings.frequencyPenalty);

  if (chatJson.open(QIODevice::ReadOnly | QIODevice::Text)
      || !chatJson.exists())
  {
    QByteArray jsonData = chatJson.readAll();
    chatJson.close();

    if (chatJson.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QJsonDocument document = QJsonDocument::fromJson(jsonData);
      QJsonArray chats = document.array();
      chats.append(object);
      document.setArray(chats);
      chatJson.write(document.toJson());
      chatJson.close();
      ChatItem *chatItem = new ChatItem(m_ui->chatList, name,
                                        m_ui->chatList->count());
      QListWidgetItem *item = new QListWidgetItem(m_ui->chatList);
      item->setSizeHint(QSize(0, 50));

      connect(chatItem, &ChatItem::removed,
              this, &MainWindow::chatItemDeleteClicked);

      m_ui->chatList->setItemWidget(item, chatItem);
      return true;
    }
  }

  return false;
}

void MainWindow::showChat()
{
	for (quint8 i = 0; i < m_ui->historyList->count(); i++)
	{
		QListWidgetItem *listItem = m_ui->historyList->item(i);
		QWidget *itemWidget = m_ui->historyList->itemWidget(listItem);
		MessageWidget *indexItem = dynamic_cast<MessageWidget *>(itemWidget);

		if (indexItem != nullptr)
		{
			delete indexItem;
		}

		delete listItem;
	}

  m_ui->historyList->clear();
  m_allMesages.clear();
  setChatSettings(m_ui->chatList->currentRow());

  if (m_chatSettings.fileName != "")
  {
    HistoryParser history(this, m_chatSettings.fileName);
    quint16 count = history.getCountMessage();
    tokensLeft();

    for (quint16 i = 0; i < count; i++)
    {
      m_allMesages.append(history.getMessage(i));
      QListWidgetItem *item = new QListWidgetItem(m_ui->historyList);
      MessageWidget *message = new MessageWidget(item, m_allMesages[i],
                               m_ui->chatList->currentRow(), i);

      connect(message, &MessageWidget::selfDelete,
              this, &MainWindow::messageDeleteCliked);

      connect(message, &MessageWidget::resizeFinished,
              this, [=]()
      {
        m_scroll = false;
      });

      m_ui->historyList->setItemWidget(item, message);
      m_ui->historyList->setCurrentItem(item);

    }

    for (OpenAIChat *chatGPT : m_chatGPTList)
    {
      if (chatGPT->getIndex() == m_ui->chatList->currentRow())
      {
        answerState(!chatGPT->isError());
        errorState(chatGPT->isError());
        chatGPT->deleteLater();
        m_chatGPTList.removeOne(chatGPT);
        return;
      }
    }

    answerState(false);
    errorState(m_allMesages.count() != 0 &&
               m_allMesages.last().role == HistoryParser::Role::User);
  }
}

void MainWindow::answerState(bool waitAnswer)
{
  m_waitAnswer = waitAnswer;
  m_movie.setPaused(!waitAnswer);
  m_ui->loading->setVisible(waitAnswer);
  m_ui->stopButton->setVisible(waitAnswer);
}

void MainWindow::errorState(bool error)
{
  m_errorAnswer = error;
  m_ui->retryButton->setVisible(error);
}

void MainWindow::tokensLeft()
{
  quint32 tokensLeft = m_chatSettings.maxTokens - m_chatSettings.usedTokens;
  m_ui->countToken->setText(tr("Tokens left") + ": " +
                            QString::number(tokensLeft));
}

void MainWindow::setFileChatSettings(quint8 index)
{
  QFile chatJson(QDir::currentPath() + "/chats.json");

  if (chatJson.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QByteArray jsonData = chatJson.readAll();
    chatJson.close();

    if (chatJson.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QJsonDocument document = QJsonDocument::fromJson(jsonData);
      QJsonArray chats = document.array();
      QJsonObject object = chats[index].toObject();
      object["name"] = m_chatSettings.name;
      object["file_name"] = m_chatSettings.fileName;
      object["model"] = m_chatSettings.model;
      object["used_tokens"] = (int)m_chatSettings.usedTokens;
      object["max_tokens"] = (int)m_chatSettings.maxTokens;
      object["temperature"] = m_chatSettings.temperature;
      object["n"] = m_chatSettings.n;
      QJsonArray stop = object["stop"].toArray();

      for (quint8 i = 0; i < stop.count(); i++)
      {
        stop.append(m_chatSettings.stop[i]);
      }

      object["stop"] = stop;
      object["presence_penalty"] = m_chatSettings.presencePenalty;
      object["frequency_penalty"] = m_chatSettings.frequencyPenalty;
      chats[index] = object;
      document.setArray(chats);
      chatJson.write(document.toJson());
      chatJson.close();
    }
  }
}

void MainWindow::sendMessage()
{
  Settings settings;
  OpenAIChat *chatGPT = new OpenAIChat(this, settings.openAIKey);
   m_ui->retryButton->setToolTip("");

  connect(chatGPT, &OpenAIChat::responseReceived,
          this, &MainWindow::responseReceived);
  connect(chatGPT, &OpenAIChat::replyError,
          this, &MainWindow::replyError);

  m_chatGPTList.append(chatGPT);
  answerState(true);
  errorState(false);
  setChatSettings(m_ui->chatList->currentRow());
  chatGPT->send(m_allMesages, m_chatSettings,
                m_ui->chatList->currentRow());
}

void MainWindow::sendClicked()
{
  receivedText(m_ui->textInput->toPlainText());
}

void MainWindow::newChatClicked()
{
  createChat(tr("New chat"));
}

void MainWindow::chatItemChanged(QListWidgetItem *item)
{
  QWidget *itemWidget = m_ui->chatList->itemWidget(item);
  ChatItem *chatItem = dynamic_cast<ChatItem *>(itemWidget);

  if (chatItem != nullptr)
  {
    chatItem->setNewMessage(false);
    showChat();
  }
}

void MainWindow::settingsClicked()
{
  SettingsWidget *settings = new SettingsWidget(this);
  settings->show();
}

void MainWindow::retryClicked()
{
  m_ui->retryButton->setToolTip("");
  answerState(true);
  errorState(false);
  sendMessage();
}

void MainWindow::stopClicked()
{
  answerState(false);
  errorState(true);

  for (OpenAIChat *chatGPT : qAsConst(m_chatGPTList))
  {
    if (chatGPT->getIndex() == m_ui->chatList->currentRow())
    {
      chatGPT->stopChat();
      chatGPT->deleteLater();
      m_chatGPTList.removeOne(chatGPT);
      break;
    }
  }
}

void MainWindow::chatSettingsClicked()
{
  ChatSettingsDialog *chatSettings = new ChatSettingsDialog(this,
                                     m_ui->chatList->currentRow());
  chatSettings->show();
}

void MainWindow::responseReceived(QString response)
{
  OpenAIChat *sender = qobject_cast<OpenAIChat*>(QObject::sender());
  m_answerEffect.play();
  disconnect(sender, &OpenAIChat::responseReceived,
          this, &MainWindow::responseReceived);
  disconnect(sender, &OpenAIChat::replyError,
          this, &MainWindow::replyError);

  ChatSettings chatSettings = m_chatSettings;
  setChatSettings(sender->getIndex());
  HistoryParser history(this, m_chatSettings.fileName);
  m_chatSettings.usedTokens = sender->getUsedToken();
  setFileChatSettings(sender->getIndex());
  history.addNewMessage(sender->getAnswerMessage());

  if (sender->getIndex() == m_ui->chatList->currentRow())
  {
    answerState(false);
    m_allMesages.append(sender->getAnswerMessage());
    QListWidgetItem *item = new QListWidgetItem(m_ui->historyList);
    MessageWidget *messageWidget = new MessageWidget(item,
                                   sender->getAnswerMessage(),
                                   m_ui->chatList->currentRow(),
                                   m_ui->historyList->count()-1);

    connect(messageWidget, &MessageWidget::selfDelete,
            this, &MainWindow::messageDeleteCliked);

    m_ui->historyList->setItemWidget(item, messageWidget);
    m_ui->historyList->setCurrentItem(item);
    m_chatSettings.usedTokens = sender->getUsedToken();
    tokensLeft();
  }
  else
  {
    QListWidgetItem *item = m_ui->chatList->item(sender->getIndex());
    QWidget *itemWidget = m_ui->chatList->itemWidget(item);
    ChatItem *chatItem = dynamic_cast<ChatItem *>(itemWidget);
    chatItem->setNewMessage(true);
    m_chatSettings = chatSettings;
  }

  sender->deleteLater();
  m_chatGPTList.removeOne(sender);
}

void MainWindow::replyError(QString error)
{
  OpenAIChat *sender = qobject_cast<OpenAIChat*>(QObject::sender());

  disconnect(sender, &OpenAIChat::responseReceived,
          this, &MainWindow::responseReceived);
  disconnect(sender, &OpenAIChat::replyError,
          this, &MainWindow::replyError);

  m_errorEffect.play();
  m_ui->retryButton->setToolTip(error);

  if (sender->getIndex() == m_ui->chatList->currentRow())
  {
    answerState(false);
    errorState(true);
  }

  sender->deleteLater();
  m_chatGPTList.removeOne(sender);
}

void MainWindow::chatItemDeleteClicked()
{
  ChatItem *sender = qobject_cast<ChatItem*>(QObject::sender());
  quint8 index = sender->getIndex();
  bool currentItem = m_ui->chatList->currentItem() ==
                     m_ui->chatList->item(index);
	delete m_ui->chatList->item(index);

  while (index < m_ui->chatList->count())
  {
    QListWidgetItem *listItem = m_ui->chatList->item(index);
    QWidget *itemWidget = m_ui->chatList->itemWidget(listItem);
    ChatItem *indexItem = dynamic_cast<ChatItem *>(itemWidget);
    indexItem->setIndex(index);
    index++;
  }

  if (checkExistChats())
  {
    if (currentItem)
    {
      m_ui->chatList->setCurrentRow(0);
      chatItemChanged(m_ui->chatList->item(0));
    }
  }

  for (OpenAIChat *chatGPT : m_chatGPTList)
  {
    if (chatGPT->getIndex() == index && chatGPT != nullptr)
    {
      chatGPT->deleteLater();
      m_chatGPTList.removeOne(chatGPT);
      break;
    }
  }
}

void MainWindow::messageDeleteCliked()
{
  MessageWidget *sender = qobject_cast<MessageWidget*>(QObject::sender());
  quint8 index = sender->getIndex();
  delete m_ui->historyList->item(index);
  m_allMesages.removeAt(index);

  while (index < m_ui->historyList->count())
  {
    QListWidgetItem *listItem = m_ui->historyList->item(index);
    QWidget *itemWidget = m_ui->historyList->itemWidget(listItem);
    MessageWidget *indexItem = dynamic_cast<MessageWidget *>(itemWidget);
    indexItem->setIndex(index);
    index++;
  }

  bool find = false;

  for (OpenAIChat *chatGPT : m_chatGPTList)
  {
    if (chatGPT->getIndex() == m_ui->chatList->currentRow())
    {
      find = true;
    }
  }

  if (!find)
  {
    errorState(m_allMesages.count() != 0 &&
               m_allMesages.last().role == HistoryParser::Role::User);
  }
}
