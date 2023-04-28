#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "NewTextEdit.h"
#include "SettingsWidget.h"
#include "ThemeIcon.h"
#include "ChatSettingsWidget.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), m_ui(new Ui::MainWindow)
{
	m_ui->setupUi(this);

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

	m_answerEffect = new QSoundEffect(this);
	m_answerEffect->setSource(QUrl::fromLocalFile(":/sound/message.wav"));
	m_answerEffect->setVolume(0.20f);
	m_errorEffect = new QSoundEffect(this);
	m_errorEffect->setSource(QUrl::fromLocalFile(":/sound/error.wav"));
	m_errorEffect->setVolume(0.15f);
}

MainWindow::~MainWindow()
{
	delete m_ui;
	delete m_answerEffect;
	delete m_errorEffect;
}

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
			sendMessage();
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
			showChat();
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
	QFile chatJson(QDir::currentPath() + "/chats.json");
	QJsonObject object;
	object.insert("name", name);
	QString uniqueFileName = QUuid::createUuid().
													 toString(QUuid::WithoutBraces).left(4);
	object.insert("file_name", uniqueFileName);
	object.insert("model", "gpt-3.5-turbo");
	object.insert("used_tokens", 0);
	object.insert("max_tokens", 4096);
	object.insert("temperature", 1);
	object.insert("n", 1);
	QJsonArray stop;
	object.insert("stop", stop);
	object.insert("presence_penalty", 0);
	object.insert("frequency_penalty", 0);

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

		for (OpenAIChat *chatGPT : m_chatGPT)
		{
			if (chatGPT->getIndex() == m_ui->chatList->currentRow())
			{
				answerState(!chatGPT->isError());
				errorState(chatGPT->isError());
				chatGPT->deleteLater();
				m_chatGPT.removeOne(chatGPT);
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
	SettingsWidget settings;
	OpenAIChat *chatGPT = new OpenAIChat(this);
	chatGPT->setKey(settings.getOpenAIKey());

	connect(chatGPT, &OpenAIChat::responseReceived,
					this, &MainWindow::responseReceived);
	connect(chatGPT, &OpenAIChat::replyError,
					this, &MainWindow::replyError);

	m_chatGPT.append(chatGPT);
	answerState(true);
	errorState(false);
	setChatSettings(m_ui->chatList->currentRow());
	chatGPT->chat(m_allMesages, m_chatSettings,
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
	answerState(true);
	errorState(false);
	sendMessage();
}

void MainWindow::stopClicked()
{
	answerState(false);
	errorState(true);

	for (OpenAIChat *chatGPT : qAsConst(m_chatGPT))
	{
		if (chatGPT->getIndex() == m_ui->chatList->currentRow())
		{
			chatGPT->stopChat();
			quint8 index = m_chatGPT.indexOf(chatGPT);
			m_chatGPT[index]->deleteLater();
			m_chatGPT.removeAt(index);
			break;
		}
	}
}

void MainWindow::chatSettingsClicked()
{
	ChatSettingsWidget *chatSettings = new ChatSettingsWidget(this,
																		 m_ui->chatList->currentRow());
	chatSettings->show();
}

void MainWindow::responseReceived(OpenAIChat *chatGPT, QString response)
{
	m_answerEffect->play();
	disconnect(chatGPT, &OpenAIChat::responseReceived,
					this, &MainWindow::responseReceived);
	disconnect(chatGPT, &OpenAIChat::replyError,
					this, &MainWindow::replyError);

	ChatSettings chatSettings = m_chatSettings;
	setChatSettings(chatGPT->getIndex());
	HistoryParser history(this, m_chatSettings.fileName);
	m_chatSettings.usedTokens = chatGPT->getUsedToken();
	setFileChatSettings(chatGPT->getIndex());
	history.addNewMessage(chatGPT->getAnswerMessage());

	if (chatGPT->getIndex() == m_ui->chatList->currentRow())
	{
		answerState(false);
		m_allMesages.append(chatGPT->getAnswerMessage());
		QListWidgetItem *item = new QListWidgetItem(m_ui->historyList);
		MessageWidget *messageWidget = new MessageWidget(item,
																	 chatGPT->getAnswerMessage(),
																	 m_ui->chatList->currentRow(),
																	 m_ui->historyList->count()-1);

		connect(messageWidget, &MessageWidget::selfDelete,
						this, &MainWindow::messageDeleteCliked);

		m_ui->historyList->setItemWidget(item, messageWidget);
		m_ui->historyList->setCurrentItem(item);
		m_chatSettings.usedTokens = chatGPT->getUsedToken();
		tokensLeft();
	}
	else
	{
		QListWidgetItem *item = m_ui->chatList->item(chatGPT->getIndex());
		QWidget *itemWidget = m_ui->chatList->itemWidget(item);
		ChatItem *chatItem = dynamic_cast<ChatItem *>(itemWidget);
		chatItem->setNewMessage(true);
		m_chatSettings = chatSettings;
	}

	quint8 index = m_chatGPT.indexOf(chatGPT);
	m_chatGPT[index]->deleteLater();
	m_chatGPT.removeAt(index);
}

void MainWindow::replyError(OpenAIChat *chatGPT, QString error)
{
	m_errorEffect->play();
	disconnect(chatGPT, &OpenAIChat::responseReceived,
					this, &MainWindow::responseReceived);
	disconnect(chatGPT, &OpenAIChat::replyError,
					this, &MainWindow::replyError);

	if (chatGPT->getIndex() == m_ui->chatList->currentRow())
	{
		answerState(false);
		errorState(true);
	}
}

void MainWindow::chatItemDeleteClicked(ChatItem *item)
{
	quint8 index = item->getIndex();
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

	for (quint8 i = 0; i < m_ui->historyList->count(); i++)
	{
		QListWidgetItem *listItem = m_ui->historyList->item(i);
		QWidget *itemWidget = m_ui->historyList->itemWidget(listItem);
		MessageWidget *indexItem = dynamic_cast<MessageWidget *>(itemWidget);
		indexItem->setChatIndex(m_ui->chatList->currentRow());
	}
}

void MainWindow::messageDeleteCliked(MessageWidget *messageWidget)
{
	quint8 index = messageWidget->getIndex();
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

	for (OpenAIChat *chatGPT : m_chatGPT)
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
