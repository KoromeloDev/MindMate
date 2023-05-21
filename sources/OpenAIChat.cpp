#include "OpenAIChat.h"

OpenAIChat::OpenAIChat(QObject *parent, QString key) : QObject(parent)
{
  m_key = key;
}

OpenAIChat::~OpenAIChat()
{
  if (m_networkManager != nullptr)
  {
    m_networkManager->deleteLater();
  }
}

void OpenAIChat::send(QList<HistoryParser::Message> message,
                      ChatSettings chatSettings, quint8 index)
{
  if (message.empty())
  {
    m_error = true;
    emit replyError("");
    return;
  }

  m_index = index;
  m_message = message;
  m_chatSettings = chatSettings;

  QJsonObject json;
  json.insert("model", m_chatSettings.model);
  json.insert("temperature", m_chatSettings.temperature);
  json.insert("n", m_chatSettings.n);
  json.insert("presence_penalty", m_chatSettings.presencePenalty);
  json.insert("frequency_penalty", m_chatSettings.frequencyPenalty);
  QJsonArray messages;
  QJsonObject messageObject;

  for (quint16 i = 0; i < m_message.size(); i++)
  {
    messageObject.insert("role", QVariant::fromValue(m_message[i].role).
                   toString().toLower());
    messageObject.insert("content", m_message[i].content);
    messages.append(messageObject);
  }

  json.insert("messages", messages);
  sendJson(json);
}

void OpenAIChat::send(QString message, ChatSettings chatSettings)
{
  if (message.isEmpty())
  {
    m_error = true;
    emit replyError("");
    return;
  }

  HistoryParser::Message messageStruct;
  messageStruct.content = message;
  messageStruct.role = HistoryParser::Role::User;
  m_message.append(messageStruct);
  m_chatSettings.model = chatSettings.model;

  QJsonObject json;
  QJsonArray messages;
  QJsonObject messageObject;
  json.insert("model", m_chatSettings.model);
  messageObject.insert("role", QVariant::fromValue(m_message[0].role).
                 toString().toLower());
  messageObject.insert("content", m_message[0].content);
  messages.append(messageObject);
  json.insert("messages", messages);
  sendJson(json);
}

void OpenAIChat::stopChat()
{
  if (m_networkManager != nullptr)
  {
    disconnect(m_networkManager, &QNetworkAccessManager::finished,
               this, &OpenAIChat::onFinished);

    m_networkManager->deleteLater();
    m_networkManager = nullptr;
  }
}

void OpenAIChat::onFinished(QNetworkReply *reply)
{
  QByteArray data = reply->readAll();
  stopChat();

  if (reply->error() == QNetworkReply::NoError ||
      reply->error() == QNetworkReply::UnknownProxyError)
  {
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject json = document.object();
    QJsonObject usage = json.value("usage").toObject();
    m_usedToken = usage["total_tokens"].toInt();
    QJsonArray choices =  json.value("choices").toArray();
    QJsonObject message = choices[0].toObject().value("message").toObject();
    QString content = message["content"].toString();

    if (!content.isEmpty())
    {
      QString role = message["role"].toString();
      role = role.at(0).toUpper() + role.mid(1);
      m_answerMessage.content = content;
      m_answerMessage.role = static_cast<HistoryParser::Role>(
                             QMetaEnum::fromType<HistoryParser::Role>().
                             keysToValue(role.toUtf8().constData(), nullptr));
      m_error = false;
      emit responseReceived(data);
      return;
    }
  }

  QJsonDocument document = QJsonDocument::fromJson(data);
  QJsonObject json = document.object();
  QJsonObject error = json.value("error").toObject();
  QString message = error.value("message").toString();
  m_error = true;
  emit replyError(message);
}

void OpenAIChat::sendJson(QJsonObject json)
{
  if (m_networkManager != nullptr)
  {
    m_error = true;
    emit replyError("");
    return;
  }

  m_networkManager = new QNetworkAccessManager(this);

  connect(m_networkManager, &QNetworkAccessManager::finished,
          this, &OpenAIChat::onFinished);

  QUrl url("https://api.openai.com/v1/chat/completions");
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  request.setRawHeader("Authorization", "Bearer " + m_key.toUtf8());
  QJsonDocument document(json);
  QByteArray postData = document.toJson(QJsonDocument::Compact);
  m_networkManager->post(request, postData);
}

quint8 OpenAIChat::getIndex() const
{
  return m_index;
}

bool OpenAIChat::isError() const
{
  return m_error;
}

quint32 OpenAIChat::getUsedToken() const
{
  return m_usedToken - 1;
}

HistoryParser::Message OpenAIChat::getAnswerMessage() const
{
  return m_answerMessage;
}
