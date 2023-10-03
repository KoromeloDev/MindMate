#ifndef NEWQLISTWIDGET_H
#define NEWQLISTWIDGET_H

#include <QListWidget>
#include <QScrollBar>
#include <QToolButton>
#include <QGridLayout>

class NewQListWidget : public QListWidget
{
  Q_OBJECT
public:
  NewQListWidget(QWidget *parent = nullptr);

private:
  QSharedPointer<QToolButton> m_downButton;

  void createDownButton();

private slots:
  void resizeDownButton();
  void downButtonClicked();
};

#endif // NEWQLISTWIDGET_H
