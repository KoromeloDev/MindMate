#ifndef HISTORYPARSER_H
#define HISTORYPARSER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QMetaEnum>

class HistoryParser : public QObject
{
  Q_OBJECT

public:
  explicit HistoryParser(QString fileName = 0);

  enum Role
  {
    User,
    Assistant,
    System
  };
  Q_ENUM(Role)

  struct Messages
  {
    Role role;
    QVector<QString> content;
    quint8 selected;

    QString getMessage() const;
    void setMessage(QString message);
    void addMessage(QString message);
  };
  struct Message
  {
    operator Messages() const;
    Message &operator=(Messages messages);
    Role role;
    QString content;
  };

  void addContent(quint16 index, QString content);
  void addMessage(Message message);
  void addMessage(Messages message);
  void editMessage(quint16 index, quint8 number, QString content);
  void deleteMessage(quint16 index);
  void deleteMessage(quint16 index, quint8 number);
  void setSelected(quint16 index, quint8 selected);
  Messages getMessages(quint16 index);
  quint16 getCountMessage();

private:
  QFile m_history;
};

#endif // HISTORYPARSER_H
