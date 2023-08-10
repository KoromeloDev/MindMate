#ifndef NEWLISTWIDGETITEM_H
#define NEWLISTWIDGETITEM_H

#include <QListWidgetItem>

class NewListWidgetItem : public QListWidgetItem
{
public:
  NewListWidgetItem(QListWidget *parent);
  int getIndex() const;

private:
  QListWidget *m_listWidget;
};

#endif // NEWLISTWIDGETITEM_H
