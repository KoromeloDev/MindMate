#include "NewListWidgetItem.h"

NewListWidgetItem::NewListWidgetItem(QListWidget *parent) : QListWidgetItem(parent)
{
  m_listWidget = parent;
}

int NewListWidgetItem::getIndex() const
{
  return m_listWidget != nullptr ? m_listWidget->row(this) : -1;
}
