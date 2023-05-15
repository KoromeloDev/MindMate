#include "SettingsWidget.h"
#include "ui_SettingsWidget.h"

#include "EditDialog.h"
#include "ThemeIcon.h"

SettingsWidget::SettingsWidget(QWidget *parent)
: QDialog(parent), m_ui(new Ui::SettingsWidget)
{
  m_ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  QString version = QApplication::applicationVersion();
  m_ui->version->setText(tr("Version") + ": " + version);
  m_settings.readSettings();
  m_chatSettingsWidget = new ChatSettingsWidget(this, m_settings.chatSettings);
  m_ui->verticalLayout_2->addWidget(m_chatSettingsWidget);

  #if CHECKUPDATES
  m_ui->checkUpdates->setChecked(m_settings.checkUpdates);
  #else
  m_ui->checkUpdates->setHidden(true);
  m_ui->updatesLabel->setHidden(true);
  #endif

  m_ui->langRecognize->setChecked(m_settings.languageRecognize);
  m_ui->autoNamingChat->setChecked(m_settings.autoNaming);

  if (m_settings.openAIKey.left(3) == "sk-" &&
      m_settings.openAIKey.length() == 51)
  {
    QString keyHidden = m_settings.openAIKey.left(3) + "..." +
                        m_settings.openAIKey.right(4);
    m_ui->keyLabel->setText(tr("API key") + ": " + keyHidden);
  }

  ThemeIcon::setIcon(*m_ui->editKeyButton, ":/icons/edit.svg");

  connect(m_ui->editKeyButton, &QToolButton::clicked,
          this, &SettingsWidget::editKeyClicked);
  connect(m_ui->autoNamingChat, &QCheckBox::stateChanged,
          this, &SettingsWidget::autoNamingStateChanged);
  connect(m_ui->langRecognize, &QCheckBox::stateChanged,
          this, &SettingsWidget::languageRecognizeStateChanged);

  #if CHECKUPDATES
  connect(m_ui->checkUpdates, &QCheckBox::stateChanged,
          this, &SettingsWidget::checkUpdatesStateChanged);
  #endif
}

SettingsWidget::~SettingsWidget()
{
  delete m_ui;
  m_chatSettingsWidget->deleteLater();
}

void SettingsWidget::closeEvent(QCloseEvent *event)
{
  m_settings.chatSettings = m_chatSettingsWidget->getSettings();
  m_settings.writeSettings();
  QWidget::closeEvent(event);
}

void SettingsWidget::editKeyClicked()
{
  QString question = tr("Enter your key from the "
                        "<a href=https://platform.openai.com/account/api-keys>"
                        "OpenAI site</a>:");
  EditDialog *dialog = new EditDialog(this, question, "");
  dialog->setWindowTitle(tr("Set OpenAI API key"));

  connect(dialog, &EditDialog::finished, this, [=]()
  {
    dialog->deleteLater();
  });
  connect(dialog, &EditDialog::textChanged, this, [=](QString key)
  {
    if (key.left(3) == "sk-" && key.length() == 51)
    {
      m_settings.openAIKey = key;

      QString keyHidden = m_settings.openAIKey.left(3) + "..." +
                          m_settings.openAIKey.right(4);
      m_ui->keyLabel->setText(tr("API key") + ": " + keyHidden);
    }
    else
    {
      QMessageBox::critical(this, tr("Wrong key"),
                            tr("You entered the wrong key"));
    }
  });

  dialog->show();
}

void SettingsWidget::languageRecognizeStateChanged(quint8 state)
{
  m_settings.languageRecognize = state;
}

void SettingsWidget::autoNamingStateChanged(quint8 state)
{
  m_settings.autoNaming = state;
}

#if CHECKUPDATES
void SettingsWidget::checkUpdatesStateChanged(quint8 state)
{
  m_settings.checkUpdates = state;
}
#endif
