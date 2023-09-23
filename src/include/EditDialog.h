#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui
{
  class EditDialog;
}

class EditDialog : public QDialog
{
  Q_OBJECT

public:
  explicit EditDialog(QWidget *parent = nullptr, QString question = 0);
  explicit EditDialog(QWidget *parent, QString question, QString textEdit);
  ~EditDialog();

private:
  Ui::EditDialog *m_ui;

private slots:
  void buttonAccepted();

signals:
  void textChanged(const QString text);
};

#endif // EDITDIALOG_H
