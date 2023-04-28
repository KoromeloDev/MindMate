#include "OpenAIChat.h"

void OpenAIChat::chat(QList<HistoryParser::Message> message,
											ChatSettings chatSettings, quint8 index)
{
	m_index = index;
	m_message = message;
	m_chatSettings = chatSettings;
	retryChat();
}

void OpenAIChat::retryChat()
{
	m_networkManager = new QNetworkAccessManager(this);

	connect(m_networkManager, &QNetworkAccessManager::finished,
					this, &OpenAIChat::onFinished);

	QUrl url("https://api.openai.com/v1/chat/completions");
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Authorization", "Bearer " + m_key.toUtf8());
	QJsonObject json;
	json.insert("model", m_chatSettings.model);
	json.insert("temperature", m_chatSettings.temperature);
	json.insert("n", m_chatSettings.n);
	json.insert("presence_penalty", m_chatSettings.presencePenalty);
	json.insert("frequency_penalty", m_chatSettings.frequencyPenalty);
	QJsonArray messages;
	QJsonObject message;

	for (quint16 i = 0; i < m_message.size(); i++)
	{
		message.insert("role", QVariant::fromValue(m_message[i].role).
									 toString().toLower());
		message.insert("content", m_message[i].content);
		messages.append(message);
	}

	json.insert("messages", messages);
	QJsonDocument document(json);
	QByteArray postData = document.toJson(QJsonDocument::Compact);
	m_networkManager->post(request, postData);
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
	stopChat();
	QByteArray data = reply->readAll();

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
			emit responseReceived(this, data);
			return;
		}
	}

	m_error = true;
	emit replyError(this, data);
	reply->deleteLater();
}

quint8 OpenAIChat::getIndex() const
{
	return m_index;
}

bool OpenAIChat::isError()
{
	return m_error;
}

quint32 OpenAIChat::getUsedToken()
{
	return m_usedToken - 1;
}

HistoryParser::Message OpenAIChat::getAnswerMessage()
{
	return m_answerMessage;
}

void OpenAIChat::setKey(const QString &setM_key)
{
	m_key = setM_key;
}
