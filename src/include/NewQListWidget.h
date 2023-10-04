#ifndef NEWQLISTWIDGET_H
#define NEWQLISTWIDGET_H

#include <QListWidget>
#include <QScrollBar>
#include <QToolButton>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLineEdit>

#include "SearchWidget.h"

class NewQListWidget : public QListWidget
{
  Q_OBJECT
public:
  NewQListWidget(QWidget *parent = nullptr);

protected:
  void keyPressEvent(QKeyEvent *event) override;

private:
  QSharedPointer<QGridLayout> m_layout;
  QSharedPointer<QToolButton> m_downButton;
  QSharedPointer<SearchWidget> m_searchWidget;
  QVector<quint16> m_searchResult;
  quint16 m_searchSelected;

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

signals:
  void changeFocus();
};

#endif // NEWQLISTWIDGET_H
