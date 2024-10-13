#include "ChatGPT.h"

ChatGPT::ChatGPT(QString key)
{
  m_key = key;
  m_networkManager = nullptr;
}

ChatGPT::~ChatGPT()
{
  if (m_networkManager != nullptr)
  {
    m_networkManager->deleteLater();
  }
}

void ChatGPT::send(QVector<HistoryParser::Messages> message,
                   ChatSettings chatSettings)
{
  if (message.empty())
  {
    m_error = true;
    emit replyError("");
    return;
  }

  m_message = message;
  m_chatSettings = chatSettings;

  QJsonObject json;
  json.insert("model", m_chatSettings.model);
  json.insert("temperature", m_chatSettings.temperature);
  json.insert("n", m_chatSettings.n);
  QJsonArray stop = QJsonArray::fromStringList(m_chatSettings.stop);
  json.insert("stop", stop);
  json.insert("presence_penalty", m_chatSettings.presencePenalty);
  json.insert("frequency_penalty", m_chatSettings.frequencyPenalty);
  QJsonArray messages;
  QJsonObject messageObject;

  for (const auto &message : std::as_const(m_message))
  {
    messageObject.insert("role", QVariant::fromValue(message.role).
                   toString().toLower());
    messageObject.insert("content", message.getMessage());
    messages.append(messageObject);
  }

  json.insert("messages", messages);
  sendJson(json);
}

void ChatGPT::send(QString message, ChatSettings chatSettings)
{
  if (message.isEmpty())
  {
    m_error = true;
    emit replyError("");
    return;
  }

  m_chatSettings.model = chatSettings.model;

  QJsonObject json;
  QJsonArray messages;
  QJsonObject messageObject;
  json.insert("model", m_chatSettings.model);
  messageObject.insert("role", QVariant::fromValue(HistoryParser::Role::User).
                 toString().toLower());
  messageObject.insert("content", message);
  messages.append(messageObject);
  json.insert("messages", messages);
  sendJson(json);
}

void ChatGPT::onFinished(QNetworkReply *reply)
{
  disconnect(m_networkManager, &QNetworkAccessManager::finished,
             this, &ChatGPT::onFinished);

  QByteArray data = reply->readAll();

  if (data.isEmpty())
  {
    emit replyError(tr("Connection failed"));
    return;
  }
  else if (reply->error() == QNetworkReply::NoError ||
           reply->error() == QNetworkReply::UnknownProxyError)
  {
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject json = document.object();
    QJsonObject usage = json.value("usage").toObject();
    m_usedToken = usage["total_tokens"].toInt();
    QJsonArray choices =  json.value("choices").toArray();
    QVector<QString> content;
    QString role;

    for (quint8 i = 0; i < choices.count(); ++i)
    {
      QJsonObject message = choices[i].toObject().value("message").toObject();
      role =  message["role"].toString();
      content.append(message["content"].toString());
    }

    if (!content.isEmpty())
    {
      role = role.at(0).toUpper() + role.mid(1);
      m_answerMessage.selected = 0;
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

void ChatGPT::sendJson(QJsonObject json)
{
  if (m_networkManager != nullptr)
  {
    m_error = true;
    emit replyError(tr("The process is busy"));
    return;
  }

  m_networkManager = new QNetworkAccessManager(this);

  connect(m_networkManager, &QNetworkAccessManager::finished,
          this, &ChatGPT::onFinished);

  QUrl url("https://api.openai.com/v1/chat/completions");
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  request.setRawHeader("Authorization", "Bearer " + m_key.toUtf8());
  QJsonDocument document(json);
  QByteArray postData = document.toJson(QJsonDocument::Compact);
  m_networkManager->post(request, postData);
}

bool ChatGPT::isError() const
{
  return m_error;
}

quint32 ChatGPT::getUsedToken() const
{
//  API gives out one more token
  return m_usedToken - 1;
}

HistoryParser::Messages ChatGPT::getAnswerMessage() const
{
  return m_answerMessage;
}
