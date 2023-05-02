#include "HistoryParser.h"

HistoryParser::HistoryParser(QObject *parent, QString fileName)
: QObject(parent)
{
	m_history.setFileName(QDir::currentPath() + "/Chat/" +
												fileName + "_history.json");
	if (!m_history.exists())
	{
		if (m_history.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QJsonObject json;
			QJsonArray message;
			json["message"] = message;
			QJsonDocument document(json);
			m_history.write(document.toJson());
			m_history.close();
		}
	}
}

void HistoryParser::addMessage(quint16 index, QString content)
{

}

void HistoryParser::addNewMessage(Message message)
{
	if (m_history.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QByteArray jsonData = m_history.readAll();
		m_history.close();

		if (m_history.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QJsonDocument document = QJsonDocument::fromJson(jsonData);
			QJsonObject object = document.object();
			QJsonArray messageArray = object["message"].toArray();
			QJsonObject userObject;
			QString roleName = QVariant::fromValue(message.role).toString();
			userObject["role"] = roleName;
			QJsonArray contentArray;
			contentArray.append(message.content);
			userObject.insert("content", contentArray);
			messageArray.append(userObject);
			object.insert("message", messageArray);
			document.setObject(object);
			m_history.write(document.toJson());
			m_history.close();
		}
	}
}

void HistoryParser::editMessage(quint16 index, QString content)
{
	if (m_history.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QByteArray jsonData = m_history.readAll();
		m_history.close();

		if (m_history.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QJsonDocument document = QJsonDocument::fromJson(jsonData);
			QJsonObject object = document.object();
			QJsonArray messageArray = object["message"].toArray();
			QJsonObject messageObject = messageArray[index].toObject();
			QJsonArray contentArray = messageObject["content"].toArray();

			if (contentArray.count() != 0)
			{
				contentArray[0]= content;
			}

			messageObject["content"] = contentArray;
			messageArray.removeAt(index);
			messageArray.insert(index, messageObject);
			object["message"] = messageArray;
			document.setObject(object);
			m_history.write(document.toJson());
			m_history.close();
		}
	}
}

void HistoryParser::editMessage(quint16 index, quint8 number, QString content)
{

}

void HistoryParser::deleteMessage(quint16 index)
{
	if (m_history.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QByteArray jsonData = m_history.readAll();
		m_history.close();

		if (jsonData.isEmpty())
		{
			return;
		}

		if (m_history.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QJsonDocument document = QJsonDocument::fromJson(jsonData);
			QJsonObject object = document.object();
			QJsonArray messageArray = object["message"].toArray();
			messageArray.removeAt(index);
			object["message"] = messageArray;
			document.setObject(object);
			m_history.write(document.toJson());
			m_history.close();
		}
	}
}

void HistoryParser::deleteMessage(quint16 index, quint8 number)
{

}

HistoryParser::Message HistoryParser::getMessage(quint16 index)
{
	Message messageStruct;

	if (m_history.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QByteArray jsonData = m_history.readAll();
		m_history.close();
		QJsonDocument document = QJsonDocument::fromJson(jsonData);
		QJsonObject object = document.object();
		QJsonArray message = object["message"].toArray();
		QJsonObject userObject = message[index].toObject();
		QJsonArray content = userObject["content"].toArray();
		messageStruct = getMessage(index, content.size()-1);
	}

	return messageStruct;
}

HistoryParser::Message HistoryParser::getMessage(quint16 index, quint8 number)
{
	Message messageStruct;
	Message messageStruct2;
	messageStruct2.role = HistoryParser::Role::Assistant;

	if (m_history.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QByteArray jsonData = m_history.readAll();
		m_history.close();
		QJsonDocument document = QJsonDocument::fromJson(jsonData);
		QJsonObject object = document.object();
		QJsonArray message = object["message"].toArray();
		QJsonObject userObject = message[index].toObject();

		messageStruct.role = static_cast<Role>(
												 QMetaEnum::fromType<Role>().keysToValue(
												 userObject["role"].toString().toUtf8().
												 constData(), nullptr));
		QJsonArray content = userObject["content"].toArray();
		messageStruct.content = content[number].toString();
	}

	return messageStruct;
}

quint16 HistoryParser::getCountMessage()
{
	if (m_history.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QByteArray jsonData = m_history.readAll();
		m_history.close();
		QJsonDocument document = QJsonDocument::fromJson(jsonData);
		QJsonObject object = document.object();
		QJsonArray message = object["message"].toArray();
		m_countMessage = message.size();
	}

	return m_countMessage;
}
