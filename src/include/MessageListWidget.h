#ifndef MESSAGELISTWIDGET_H
#define MESSAGELISTWIDGET_H

#include <NewQListWidget.h>
#include "HistoryParser.h"
#include "MessageWidget.h"

class MessageListWidget : public NewQListWidget
{
  Q_OBJECT
public:
  MessageListWidget(QWidget *parent = nullptr);

  MessageWidget *addMessageWidget(HistoryParser::Messages message,
                                  quint8 chatIndex);

protected:
  QVector<QString> searchItems() override;

protected slots:
  void itemDelete() override;

signals:
  void messageDelete();
};

#endif // MESSAGELISTWIDGET_H
