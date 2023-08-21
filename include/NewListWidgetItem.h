#ifndef NEWLISTWIDGETITEM_H
#define NEWLISTWIDGETITEM_H

#include <QListWidgetItem>

class NewListWidgetItem : public QListWidgetItem
{
public:
  explicit NewListWidgetItem(QListWidget *parent = nullptr);
  int getIndex() const;

private:
  QListWidget *m_listWidget;
};

#endif // NEWLISTWIDGETITEM_H
