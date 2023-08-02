#ifndef EDITMESSAGEDIALOG_H
#define EDITMESSAGEDIALOG_H

#include <QDialog>

#include "MessageWidget.h"

namespace Ui
{
  class EditMessageDialog;
}

class EditMessageDialog : public QDialog
{
  Q_OBJECT

public:
  explicit EditMessageDialog(QWidget *parent = nullptr,
                             MessageWidget *messageWidget = nullptr);
  ~EditMessageDialog();

  MessageWidget *getMessageWidget() const;

protected:
  void changeEvent(QEvent *e) override;

private:
  Ui::EditMessageDialog *m_ui;
  MessageWidget *m_messageWidget;
  QSize minSize;
  QSize maxSize;

  void resize(QSize size);

};

#endif // EDITMESSAGEDIALOG_H
