#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "APIKeyWidget.h"
#include "ChatGPT.h"

namespace Ui
{
  class SetupDialog;
}

class SetupDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SetupDialog(QWidget *parent = nullptr);
  ~SetupDialog();

private:
  Ui::SetupDialog *m_ui;
  QSharedPointer<APIKeyWidget> m_keyWidget;
  ChatGPT *chatGPT;

private slots:
  void keyChanged();
  void replyError(QString error);
  void responseReceived(QString responce);
};

#endif // SETUPDIALOG_H
