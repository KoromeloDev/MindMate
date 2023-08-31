#include "HistoryParser.h"

HistoryParser::HistoryParser(QString fileName)
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

void HistoryParser::addContent(quint16 index, QString content)
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
      QJsonObject userObject = messageArray[index].toObject();
      QJsonArray contentArray = userObject["content"].toArray();
      contentArray.append(content);
      userObject.insert("content", contentArray);
      messageArray[index] = userObject;
      object.insert("message", messageArray);
      document.setObject(object);
      m_history.write(document.toJson());
      m_history.close();
    }
  }
}

void HistoryParser::addMessage(Message message)
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
      QJsonArray contentArray;
      QString roleName = QVariant::fromValue(message.role).toString();
      userObject["role"] = roleName;
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

void HistoryParser::addMessage(Messages message)
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
      QJsonArray contentArray = QJsonArray::fromStringList(message.content);
      QString roleName = QVariant::fromValue(message.role).toString();
      userObject["role"] = roleName;
      userObject.insert("content", contentArray);
      messageArray.append(userObject);
      object.insert("message", messageArray);
      document.setObject(object);
      m_history.write(document.toJson());
      m_history.close();
    }
  }
}

void HistoryParser::editMessage(quint16 index, quint8 number, QString content)
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
        contentArray[number] = content;
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
      QJsonObject userObject = messageArray[index].toObject();
      QJsonArray contentArray = userObject["content"].toArray();
      quint8 selected = userObject["selected"].toInt();
      contentArray.removeAt(number);

      if (selected != 0 && contentArray.size() == selected)
      {
        userObject["selected"] = selected - 1;
      }

      userObject["content"] = contentArray;
      messageArray[index] = userObject;
      object["message"] = messageArray;
      document.setObject(object);
      m_history.write(document.toJson());
      m_history.close();
    }
  }
}

void HistoryParser::setSelected(quint16 index, quint8 selected)
{
  if (m_history.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QByteArray jsonData = m_history.readAll();
    m_history.close();

    if (m_history.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QJsonDocument document = QJsonDocument::fromJson(jsonData);
      QJsonObject object = document.object();
      QJsonArray message = object["message"].toArray();
      QJsonObject userObject = message[index].toObject();
      userObject["selected"] = selected;
      message.removeAt(index);
      message.insert(index, userObject);
      object.insert("message", message);
      document.setObject(object);
      m_history.write(document.toJson());
      m_history.close();
    }
  }
}

HistoryParser::Messages HistoryParser::getMessages(quint16 index)
{
  Messages messagesStruct;

  if (m_history.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QByteArray jsonData = m_history.readAll();
    m_history.close();
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    QJsonObject object = document.object();
    QJsonArray message = object["message"].toArray();
    QJsonObject userObject = message[index].toObject();
    QJsonArray content = userObject["content"].toArray();
    messagesStruct.role = static_cast<Role>(
                          QMetaEnum::fromType<Role>().keysToValue(
                          userObject["role"].toString().toUtf8().
                          constData(), nullptr));
    messagesStruct.selected = userObject["selected"].toInt();

    for (quint8 i = 0; i < content.size(); ++i)
    {
      messagesStruct.content.append(content[i].toString());
    }
  }

  return messagesStruct;
}

quint16 HistoryParser::getCountMessage()
{
  quint16 countMessage = 0;

  if (m_history.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QByteArray jsonData = m_history.readAll();
    m_history.close();
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    QJsonObject object = document.object();
    QJsonArray message = object["message"].toArray();
    countMessage = message.size();
  }

  return countMessage;
}

HistoryParser::Message::operator Messages() const
{
  Messages messages;
  messages.role = this->role;
  messages.content.append(this->content);
  messages.selected = 0;
  return messages;
}

HistoryParser::Message &HistoryParser::Message::operator=(Messages messages)
{
  this->role = messages.role;
  this->content = messages.getMessage();
  return *this;
}

QString HistoryParser::Messages::getMessage() const
{
  return content[selected];
}

void HistoryParser::Messages::setMessage(QString message)
{
  content[selected] = message;
}

void HistoryParser::Messages::addMessage(QString message)
{
  content.append(message);
}
