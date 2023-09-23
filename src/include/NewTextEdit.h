#ifndef NEWTEXTEDIT_H
#define NEWTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <QKeyEvent>
#include <QPainter>
#include <QScrollBar>

namespace Ui
{
  class NewTextEdit;
}

class NewTextEdit : public QTextEdit
{
  Q_OBJECT

public:
  explicit NewTextEdit(QWidget *parent = nullptr);
  ~NewTextEdit();

protected:
  void keyPressEvent(QKeyEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

private:
  Ui::NewTextEdit *m_ui;

  void resizeTextInput();
  void clippingStart(QString &text);
  void clippingEnd(QString &text);
  bool isIndent(QChar symbol);

signals:
  void sendText(QString text);
};

#endif // NEWTEXTEDIT_H
