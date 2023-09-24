#include "SetupDialog.h"
#include "ui_SetupDialog.h"

#include "Settings.h"

SetupDialog::SetupDialog(QWidget *parent)
: QDialog(parent), m_ui(new Ui::SetupDialog)
{
  m_ui->setupUi(this);
  m_keyWidget = m_keyWidget.create(this);
  chatGPT = nullptr;
  m_keyWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_ui->gridLayout->addWidget(m_keyWidget.get(), 0, 0);

  connect(m_keyWidget.get(), &APIKeyWidget::keyChanged,
          this, &SetupDialog::keyChanged);

  connect(m_ui->doneButton, &QToolButton::clicked, this, [=]()
  {
    emit finished(0);
  });
}

SetupDialog::~SetupDialog()
{
  delete m_ui;

  if (chatGPT != nullptr)
  {
    delete chatGPT;
  }
}

void SetupDialog::keyChanged()
{
  chatGPT = new ChatGPT(m_keyWidget->getKey());

  connect(chatGPT, &ChatGPT::replyError,
          this, &SetupDialog::replyError);
  connect(chatGPT, &ChatGPT::responseReceived,
          this, &SetupDialog::responseReceived);

  chatGPT->send("hi", {});
}

void SetupDialog::replyError(QString error)
{
  m_ui->doneButton->setEnabled(false);

  if (error.contains("You didn't provide an API key"))
  {
    QMessageBox::critical(this, tr("Wrong key"),
                          tr("You didn't provide an API key"));
  }
  else if (error.contains("Incorrect API key provided"))
  {
    QMessageBox::critical(this, tr("Wrong key"),
                          tr("Incorrect API key provided"));
  }
  else
  {
    QMessageBox::critical(this, tr("Wrong key"), error);
  }
}

void SetupDialog::responseReceived(QString responce)
{
  Settings settings;
  settings.openAIKey = m_keyWidget->getKey();
  settings.writeSettings();
  m_ui->doneButton->setEnabled(true);
}
