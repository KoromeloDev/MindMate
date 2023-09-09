#include "APIKeyWidget.h"
#include "ui_APIKeyWidget.h"

#include "ThemeIcon.h"
#include "Settings.h"

APIKeyWidget::APIKeyWidget(QWidget *parent)
: QWidget(parent), m_ui(new Ui::APIKeyWidget)
{
  m_ui->setupUi(this);

  ThemeIcon::setIcon(*m_ui->editKeyButton, ":/icons/edit.svg");
  Settings settings;
  setTextKey(settings.openAIKey);

  connect(m_ui->editKeyButton, &QToolButton::clicked,
          this, &APIKeyWidget::editKeyClicked);
}

APIKeyWidget::~APIKeyWidget()
{
  delete m_ui;
}

QString APIKeyWidget::getKey() const
{
  return m_key.getKey();
}

bool APIKeyWidget::setTextKey(QString key)
{
  QString keyHidden;
  bool result = m_key.setTextKey(key, keyHidden);

  if (result)
  {
    m_ui->keyLabel->setText(tr("API key: ") + keyHidden);
  }

  return result;
}

void APIKeyWidget::editKeyClicked()
{
  QString question = tr("Enter your key from the "
                        "<a href=https://platform.openai.com/account/api-keys>"
                        "OpenAI site</a>:");
  m_editDialog = m_editDialog.create(this, question, "");
  m_editDialog->setWindowTitle(tr("Set OpenAI API key"));

  connect(m_editDialog.get(), &EditDialog::textChanged, this, [=](QString key)
  {
    if (setTextKey(key))
    {
      emit keyChanged();
    }
    else
    {
      QMessageBox::critical(this, tr("Wrong key"),
                            tr("You entered the wrong key"));
    }
  });

  m_editDialog->show();
}
