#include "MessageListWidget.h"
#include "NewListWidgetItem.h"

MessageListWidget::MessageListWidget(QWidget *parent) : NewQListWidget(parent)
{
}

MessageWidget *MessageListWidget::addMessageWidget(
              HistoryParser::Messages message, quint8 chatIndex)
{
  NewListWidgetItem *item = new NewListWidgetItem(this);
  MessageWidget *messageWidget = new MessageWidget(item, message, chatIndex);
  setItemWidget(item, messageWidget);
  scrollToBottom();

  connect(messageWidget, &MessageWidget::selfEdit,
          this, &MessageListWidget::itemEdit);
  connect(messageWidget, &MessageWidget::selfDelete,
          this, &MessageListWidget::itemDelete);

  return messageWidget;
}

QVector<QString> MessageListWidget::searchItems()
{
  QVector<QString> messages;

  for (quint16 i = 0; i < count(); ++i)
  {
    MessageWidget *messageWidget = dynamic_cast<MessageWidget*>(
                                   itemWidget(item(i)));
    if (messageWidget != nullptr)
    {
      messages.append(messageWidget->getMessages().getMessage());
    }
  }

  return messages;
}

void MessageListWidget::itemDelete()
{
  NewQListWidget::itemDelete();
  MessageWidget *sender = qobject_cast<MessageWidget*>(QObject::sender());
  const quint8 &index = sender->getItem()->getIndex();
  takeItem(index);
  if (index == 0 && count() > 0)
  {
    QListWidgetItem *item = this->item(0);
    MessageWidget *nextMessage = dynamic_cast<MessageWidget*>(
                                 itemWidget(item));
    if (nextMessage != nullptr)
    {
      nextMessage->hideGenerate();
    }
  }

  emit messageDelete();
}
