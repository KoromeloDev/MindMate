#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "NewTextEdit.h"
#include "SettingsWidget.h"
#include "ThemeIcon.h"
#include "ChatItem.h"
#include "Settings.h"
#include "NewListWidgetItem.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);

  setChatSettings(0);
  fillChatList();

  ThemeIcon::setIcon(*m_ui->newChatButton, ":/resources/icons/add.svg");
  ThemeIcon::setIcon(*m_ui->settingsButton, ":/resources/icons/settings.svg");
  ThemeIcon::setIcon(*m_ui->retryButton, ":/resources/icons/refresh.svg");
  ThemeIcon::setIcon(*m_ui->stopButton, ":/resources/icons/stop.svg");
  ThemeIcon::setIcon(*m_ui->chatSettingsButton, ":/resources/icons/configuration.svg");
  ThemeIcon::setIcon(*m_ui->sendButton, ":/resources/icons/send.svg");

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
  Settings settings;

  if(settings.checkUpdates)
  {
    m_updateChecker = m_updateChecker.create(this, "KoromeloDev", "MindMate");

    connect(m_updateChecker.get(), &UpdateChecker::needUpdates,
            this, &MainWindow::needUpdates);

    m_updateChecker->checkUpdates();
  }
  #endif

  m_isWaitAnswer = false;
  m_isErrorAnswer = false;
  m_movie.setFileName(":/resources/icons/eclipse.gif");
  m_movie.setScaledSize(QSize(32, 32));
  m_ui->loading->setMovie(&m_movie);
  m_ui->historyList->verticalScrollBar()->setSingleStep(20);

  m_answerEffect.setSource(QUrl::fromLocalFile(":/resources/sounds/message.wav"));
  m_answerEffect.setVolume(0.20f);
  m_errorEffect.setSource(QUrl::fromLocalFile(":/resources/sounds/error.wav"));
  m_errorEffect.setVolume(0.15f);
}

MainWindow::~MainWindow()
{
  delete m_ui;

  for (ChatGPT *chatGPT : m_chatGPTList)
  {
    chatGPT->deleteLater();
  }
}

#if CHECKUPDATES
void MainWindow::needUpdates(bool haveUpdates, QUrl downloadUrl)
{
  m_updateChecker.clear();

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
  emit resized();
}

void MainWindow::setChatSettings(const quint8 &index)
{
  m_chatSettings = m_chatSettings.getSettings(index);
}

void MainWindow::receivedText(QString text)
{
  if (!text.isEmpty() && ((!m_isWaitAnswer && !m_isErrorAnswer) ||
     (m_allMesages.count() != 0 && m_ui->roleBox->currentIndex() != 0)))
  {
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

    addMessage(message, m_ui->chatList->currentRow());
    history.addNewMessage(message);
    m_allMesages.append(message);
    m_ui->textInput->clear();

    if (message.role ==  HistoryParser::User)
    {
      Settings settings;

      if (m_ui->historyList->count() == 1 && settings.autoNaming)
      {
        ChatGPT *chatGPT = new ChatGPT(this, settings.openAIKey);
        ChatSettings chatSettings;
        chatSettings.stop = {"\n"};

        connect(chatGPT, &ChatGPT::responseReceived, this, [=]()
        {
          QString name = chatGPT->getAnswerMessage().content;
          chatGPT->deleteLater();

          if (!name.isEmpty())
          {
            static QRegularExpression re("\"([\\w\\W]*)\"");
            QRegularExpressionMatchIterator matchIterator = re.globalMatch(name);

            if (matchIterator.hasNext())
            {
              name = matchIterator.next().captured(1);
            }

            while (!name.isEmpty())
            {
              if (name.last(1) == ' ' || name.last(1) == '.'
                  || name.last(1) == '\n')
              {
                name.remove(name.length() - 1, 1);
              }
              else
              {
                break;
              }
            }

            if (name.length() < 30 && !name.isEmpty())
            {
              QListWidgetItem *item = m_ui->chatList->currentItem();
              QWidget *itemWidget = m_ui->chatList->itemWidget(item);
              ChatItem *chatItem = dynamic_cast<ChatItem *>(itemWidget);
              chatItem->setName(name);
            }
          }

          sendMessage();
        });
        connect(chatGPT, &ChatGPT::replyError, this, [=]()
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
           "name without unnecessary words."), chatSettings);
        errorState(false);
        answerState(true);
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

      for (quint8 i = 0; i < chats.count(); ++i)
      {
        QJsonObject object = chats[i].toObject();
        QString name = object["name"].toString();
        addChatItem(name);
      }

      m_ui->chatList->setCurrentRow(0);
      chatItemChanged(m_ui->chatList->item(0));
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

void MainWindow::addChatItem(QString name)
{
  NewListWidgetItem *item = new NewListWidgetItem(m_ui->chatList);
  ChatItem *chatItem = new ChatItem(m_ui->chatList, name, item);
  m_ui->chatList->setItemWidget(item, chatItem);
  item->setSizeHint(QSize(0, 50));

  connect(chatItem, &ChatItem::removed,
          this, &MainWindow::chatItemDeleteClicked);
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
  QJsonArray stop = QJsonArray::fromStringList(chatSettings.stop);
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
      addChatItem(name);
      return true;
    }
  }

  return false;
}

void MainWindow::showChat()
{
  m_ui->historyList->clear();
  m_allMesages.clear();
  setChatSettings(m_ui->chatList->currentRow());

  if (!m_chatSettings.fileName.isEmpty())
  {
    HistoryParser history(this, m_chatSettings.fileName);
    quint16 count = history.getCountMessage();
    tokensLeft();

    for (quint16 i = 0; i < count; ++i)
    {
      m_allMesages.append(history.getMessage(i));
      addMessage(m_allMesages[i], m_ui->chatList->currentRow());
    }

    if (count > 0)
    {
      QListWidgetItem *item =  m_ui->historyList->item(count - 1);
      MessageWidget *message = dynamic_cast<MessageWidget*>(
                              m_ui->historyList->itemWidget(item));

      if (message != nullptr)
      {
        connect(message, &MessageWidget::resizeFinished,
                this, &MainWindow::scrollToBottom);
      }
    }

    for (ChatGPT *chatGPT : m_chatGPTList)
    {
      if (chatGPT->getIndex() == m_ui->chatList->currentRow())
      {
        answerState(!chatGPT->isError());
        errorState(chatGPT->isError());
        m_chatGPTList.removeOne(chatGPT);
        chatGPT->deleteLater();
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
  m_isWaitAnswer = waitAnswer;
  m_movie.setPaused(!waitAnswer);
  m_ui->loading->setVisible(waitAnswer);
  m_ui->stopButton->setVisible(waitAnswer);
}

void MainWindow::errorState(bool error)
{
  m_isErrorAnswer = error;
  m_ui->retryButton->setVisible(error);
}

void MainWindow::tokensLeft()
{
  quint32 tokensLeft = m_chatSettings.getMaxTokens() -
                       m_chatSettings.usedTokens;
  m_ui->countToken->setText(tr("Tokens left") + ": " +
                            QString::number(tokensLeft));
}

void MainWindow::setFileChatSettings(const quint8 &index)
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
      object["max_tokens"] = (int)m_chatSettings.getMaxTokens();
      object["temperature"] = m_chatSettings.temperature;
      object["n"] = m_chatSettings.n;
      object["stop"] = QJsonArray::fromStringList(m_chatSettings.stop);
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
  ChatGPT *chatGPT = new ChatGPT(this, settings.openAIKey);
  m_ui->retryButton->setToolTip("");

  connect(chatGPT, &ChatGPT::responseReceived,
          this, &MainWindow::responseReceived);
  connect(chatGPT, &ChatGPT::replyError,
          this, &MainWindow::replyError);

  m_chatGPTList.append(chatGPT);
  answerState(true);
  errorState(false);
  setChatSettings(m_ui->chatList->currentRow());
  chatGPT->send(m_allMesages, m_chatSettings, m_ui->chatList->currentRow());
}

void MainWindow::addMessage(HistoryParser::Message message, quint8 chatIndex)
{
  NewListWidgetItem *item = new NewListWidgetItem(m_ui->historyList);
  MessageWidget *messageWidget = new MessageWidget(item, message, chatIndex);
  m_ui->historyList->setItemWidget(item, messageWidget);

  connect(this, &MainWindow::resized,
          messageWidget, &MessageWidget::resize);
  connect(messageWidget, &MessageWidget::selfDelete,
          this, &MainWindow::messageDeleteCliked);
  connect(messageWidget, &MessageWidget::selfEdit,
          this, &MainWindow::messageEdit);
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

  for ( ChatGPT *chatGPT : m_chatGPTList)
  {
    if (chatGPT->getIndex() == m_ui->chatList->currentRow())
    {
      chatGPT->deleteLater();
      m_chatGPTList.removeOne(chatGPT);
      break;
    }
  }
}

void MainWindow::chatSettingsClicked()
{
  m_chatSettingsDialog = m_chatSettingsDialog.create(
                         this, m_ui->chatList->currentRow());
  m_chatSettingsDialog->show();
}

void MainWindow::responseReceived()
{
  ChatGPT *sender = qobject_cast<ChatGPT*>(QObject::sender());
  m_answerEffect.play();

  disconnect(sender, &ChatGPT::responseReceived,
             this, &MainWindow::responseReceived);
  disconnect(sender, &ChatGPT::replyError,
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
    addMessage(sender->getAnswerMessage(), m_ui->chatList->currentRow());
    m_chatSettings.usedTokens = sender->getUsedToken();
    tokensLeft();
  }
  else
  {
    QListWidgetItem *item = m_ui->chatList->item(sender->getIndex());
    QWidget *itemWidget = m_ui->chatList->itemWidget(item);
    ChatItem *chatItem = dynamic_cast<ChatItem*>(itemWidget);
    chatItem->setNewMessage(true);
    m_chatSettings = chatSettings;
  }

  m_chatGPTList.removeOne(sender);
  sender->deleteLater();
}

void MainWindow::replyError(QString error)
{
  ChatGPT *sender = qobject_cast<ChatGPT*>(QObject::sender());

  disconnect(sender, &ChatGPT::responseReceived,
          this, &MainWindow::responseReceived);
  disconnect(sender, &ChatGPT::replyError,
          this, &MainWindow::replyError);

  m_errorEffect.play();
  m_ui->retryButton->setToolTip(error);

  if (sender->getIndex() == m_ui->chatList->currentRow())
  {
    answerState(false);
    errorState(true);
  }

  m_chatGPTList.removeOne(sender);
  sender->deleteLater();
}

void MainWindow::chatItemDeleteClicked()
{
  ChatItem *sender = qobject_cast<ChatItem*>(QObject::sender());
  const quint8 &index = sender->getItem()->getIndex();

  disconnect(m_ui->chatList, &QListWidget::currentItemChanged,
             this, &MainWindow::chatItemChanged);

  m_ui->chatList->takeItem(index);

  connect(m_ui->chatList, &QListWidget::currentItemChanged,
          this, &MainWindow::chatItemChanged);

  bool currentItem = m_ui->chatList->currentItem() ==
                     m_ui->chatList->item(index);

  if (checkExistChats())
  {
    if (currentItem)
    {
      m_ui->chatList->setCurrentRow(0);
      chatItemChanged(m_ui->chatList->item(0));
    }
  }

  for (ChatGPT *chatGPT : m_chatGPTList)
  {
    if (chatGPT->getIndex() == index)
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

  disconnect(sender, &MessageWidget::selfDelete,
             this, &MainWindow::messageDeleteCliked);

  const qint8 &index = sender->getItem()->getIndex();
  m_ui->historyList->takeItem(index);
  m_allMesages.removeAt(index);
  bool find = false;

  for (const auto &chatGPT : m_chatGPTList)
  {
    if (chatGPT->getIndex() == m_ui->chatList->currentRow())
    {
      find = true;
      break;
    }
  }

  if (!find)
  {
    errorState(m_allMesages.count() != 0 &&
               m_allMesages.last().role == HistoryParser::Role::User);
  }
}

void MainWindow::messageEdit()
{
  MessageWidget *sender = qobject_cast<MessageWidget*>(QObject::sender());
  m_allMesages[sender->getItem()->getIndex()] = sender->getMessage();

  //Fixing resize edited message widget
  for (quint8 i = 0; i < 12; ++i)
  {
    resize(size() + QSize(1, 0));
    resize(size() + QSize(-1, 0));
  }
}

void MainWindow::scrollToBottom()
{
  MessageWidget *message = qobject_cast<MessageWidget*>(QObject::sender());

  if (message->isTimerResize())
  {
    disconnect(message, &MessageWidget::resizeFinished,
               this, &MainWindow::scrollToBottom);

    m_ui->historyList->scrollToBottom();
  }
}
