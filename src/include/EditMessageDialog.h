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

  quint8 getIndex() const;

protected:
  void changeEvent(QEvent *e) override;

private:
  Ui::EditMessageDialog *m_ui;
  MessageWidget *m_messageWidget;
  QSize m_minSize;
  QSize m_maxSize;
  quint8 m_index;

  void resize(QSize size);
};

#endif // EDITMESSAGEDIALOG_H
