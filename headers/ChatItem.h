#ifndef CHATITEM_H
#define CHATITEM_H
#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "EditDialog.h"

namespace Ui
{
  class ChatItem;
}

class ChatItem : public QWidget
{
  Q_OBJECT

public:
  explicit ChatItem(QWidget *parent = nullptr, QString name = 0,
                    quint8 index = 0);
  ~ChatItem();

  quint8 getIndex() const;
  void setIndex(quint8 index);
  void setNewMessage(bool message);

  bool thereIsNew() const;
  void editName(QString name);

private:
  Ui::ChatItem *m_ui;
  QString m_name;
  quint8 m_index;
  bool m_message;
  EditDialog *m_editDialog = nullptr;

  void checkWidth();

private slots:
  void editClicked();
  void deleteCliked();

signals:
  void removed();
};
#endif // CHATITEM_H
