#ifndef CHATITEM_H
#define CHATITEM_H

#include <QWidget>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <thread>

#include "EditDialog.h"
#include "NewListWidgetItem.h"

namespace Ui
{
  class ChatItem;
}

class ChatItem : public QWidget
{
  Q_OBJECT

public:
  explicit ChatItem(QWidget *parent = nullptr, QString name = 0,
                    NewListWidgetItem *item = nullptr);
  ~ChatItem();

  bool isNew() const;
  void setNewMessage(const bool &message);
  void setName(const QString &name);
  NewListWidgetItem *getItem() const;

private:
  Ui::ChatItem *m_ui;
  QString m_name;
  NewListWidgetItem *m_item;
  bool m_message;
  QSharedPointer<EditDialog> m_editDialog;

  void checkWidth();

private slots:
  void editClicked();
  void deleteCliked();

signals:
  void removed();
};
#endif // CHATITEM_H
