#ifndef NEWQLISTWIDGET_H
#define NEWQLISTWIDGET_H

#include <QListWidget>
#include <QScrollBar>
#include <QToolButton>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QShortcut>
#include <QApplication>

#include "SearchWidget.h"

class NewQListWidget : public QListWidget
{
  Q_OBJECT
public:
  NewQListWidget(QWidget *parent = nullptr);

  void resetSeachWidget(bool hide);

private:
  QSharedPointer<QGridLayout> m_layout;
  QSharedPointer<QToolButton> m_downButton;
  QSharedPointer<SearchWidget> m_searchWidget;
  QVector<quint16> m_searchResult;
  quint16 m_searchSelected;
  QSharedPointer<QShortcut> m_shortcut;

  void createDownButton();
  void searchShow();
  void setAllFoundColor();
  void resetAllFoundColor();
  void setFoundColor(quint16 index);
  void setSelectedFoundColor(quint16 index);

private slots:
  void resizeDownButton();
  void downButtonClicked();
  void searchItems();
  void searchPageChanged(quint16 &page);
  void focusChanged(QWidget *oldFocus, QWidget *newFocus);

signals:
  void changeFocus();
};

#endif // NEWQLISTWIDGET_H
