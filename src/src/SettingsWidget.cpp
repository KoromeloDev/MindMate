#include "SettingsWidget.h"
#include "ui_SettingsWidget.h"

SettingsWidget::SettingsWidget(QWidget *parent)
: QDialog(parent), m_ui(new Ui::SettingsWidget)
{
  m_ui->setupUi(this);
  QString version = QApplication::applicationVersion();
  m_ui->version->setText(tr("Version") + ": " + version);
  m_settings.readSettings();
  m_keyWidget = m_keyWidget.create(this);
  m_ui->verticalLayout_2->insertWidget(0, m_keyWidget.get());
  m_chatSettingsWidget = m_chatSettingsWidget.create(this,
                                                     m_settings.chatSettings);
  m_ui->verticalLayout_2->addWidget(m_chatSettingsWidget.get());
  m_ui->langRecognize->setChecked(m_settings.languageRecognize);
  m_ui->autoNamingChat->setChecked(m_settings.autoNaming);

  connect(m_ui->autoNamingChat, &QCheckBox::stateChanged,
          this, &SettingsWidget::autoNamingStateChanged);
  connect(m_ui->langRecognize, &QCheckBox::stateChanged,
          this, &SettingsWidget::languageRecognizeStateChanged);
}

SettingsWidget::~SettingsWidget()
{
  delete m_ui;
}

void SettingsWidget::closeEvent(QCloseEvent *event)
{
  m_settings.openAIKey = m_keyWidget->getKey();
  m_settings.chatSettings = m_chatSettingsWidget->getSettings();
  m_settings.writeSettings();
  QWidget::closeEvent(event);
}

void SettingsWidget::languageRecognizeStateChanged(quint8 state)
{
  m_settings.languageRecognize = state;
}

void SettingsWidget::autoNamingStateChanged(quint8 state)
{
  m_settings.autoNaming = state;
}
