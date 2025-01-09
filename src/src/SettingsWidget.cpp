#include "SettingsWidget.h"
#include "ui_SettingsWidget.h"

#include <QDesktopServices>

#include "ThemeIcon.h"

SettingsWidget::SettingsWidget(QWidget *parent)
: QDialog(parent), m_ui(new Ui::SettingsWidget)
{
  m_ui->setupUi(this);
  ThemeIcon::setIcon(*m_ui->copyButton, ":/icons/copy.svg");
  ThemeIcon::setIcon(*m_ui->homepageButton, ":/icons/GitHubLogo.svg");
  ThemeIcon::setIcon(*m_ui->reportButton, ":/icons/bug.svg");
  m_clipboard = QApplication::clipboard();
  m_timer = nullptr;
  QString version = QApplication::applicationVersion();
  m_ui->version->setText(tr("App version") + ": " + version);
  m_ui->qt->setText(QString("Qt version: %1").arg(qVersion()));
  m_ui->os->setText(QString("OS: %1").arg(QSysInfo::productType()));

  if (QSysInfo::productVersion() != "unknown")
  {
    m_ui->osVersion->setText(QString("OS version: %1").arg(
                             QSysInfo::productVersion()));
  }
  else
  {
    m_ui->osVersion->setVisible(false);
  }

  m_settings.readSettings();
  m_keyWidget = m_keyWidget.create(this);
  m_ui->verticalLayout_2->insertWidget(0, m_keyWidget.get());
  m_chatSettingsWidget = m_chatSettingsWidget.create(this,
                                                     m_settings.chatSettings);
  m_ui->verticalLayout_2->addWidget(m_chatSettingsWidget.get());
  m_ui->langRecognize->setChecked(m_settings.languageRecognize);
  m_ui->autoNamingChat->setChecked(m_settings.autoNaming);

  m_ui->uColorEdit1->setText(m_settings.userMessageColor1);
  m_ui->uColorEdit2->setText(m_settings.userMessageColor2);
  m_ui->aColorEdit1->setText(m_settings.assistantMessageColor1);
  m_ui->aColorEdit2->setText(m_settings.assistantMessageColor2);
  m_ui->sColorEdit1->setText(m_settings.systemMessageColor1);
  m_ui->sColorEdit2->setText(m_settings.systemMessageColor2);
  updateColor();

  connect(m_ui->autoNamingChat, &QCheckBox::stateChanged,
          this, &SettingsWidget::autoNamingStateChanged);
  connect(m_ui->langRecognize, &QCheckBox::stateChanged,
          this, &SettingsWidget::languageRecognizeStateChanged);
  connect(m_ui->copyButton, &QToolButton::clicked,
          this, &SettingsWidget::copyClicked);
  connect(m_ui->homepageButton, &QToolButton::clicked,
          this, &SettingsWidget::homepageClicked);
  connect(m_ui->reportButton, &QToolButton::clicked,
          this, &SettingsWidget::reportClicked);
  connect(m_ui->applyThemeButton, &QToolButton::clicked,
          this, &SettingsWidget::writeColorSettings);
  connect(m_ui->uColorEdit1, &QLineEdit::textChanged,
          this, &SettingsWidget::updateColor);
  connect(m_ui->uColorEdit2, &QLineEdit::textChanged,
          this, &SettingsWidget::updateColor);
  connect(m_ui->aColorEdit1, &QLineEdit::textChanged,
          this, &SettingsWidget::updateColor);
  connect(m_ui->aColorEdit2, &QLineEdit::textChanged,
          this, &SettingsWidget::updateColor);
  connect(m_ui->sColorEdit1, &QLineEdit::textChanged,
          this, &SettingsWidget::updateColor);
  connect(m_ui->sColorEdit2, &QLineEdit::textChanged,
          this, &SettingsWidget::updateColor);
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

void SettingsWidget::copyClicked()
{
  QString info;
  info.append(QString("App version: %1 \nQt version: %2\nOS: %3").arg(
              QApplication::applicationVersion(), qVersion(),
              QSysInfo::productType()));

  if (QSysInfo::productVersion() != "unknown")
  {
    info.append(QString("\nOS version: %1").arg(QSysInfo::productVersion()));
  }

  m_clipboard->setText(info);

  if (m_timer.isNull())
  {
    m_ui->copyButton->setText(tr("Copied") + "!");
    ThemeIcon::setIcon(*m_ui->copyButton, ":/icons/accept.svg");
    m_timer = m_timer.create(this);
    m_timer->setSingleShot(true);
    m_timer->setInterval(1500);

    connect(m_timer.get(), &QTimer::timeout,
            this, &SettingsWidget::timerTimeout);

    m_timer->start();
  }
}

void SettingsWidget::timerTimeout()
{
  disconnect(m_timer.get(), &QTimer::timeout,
             this, &SettingsWidget::timerTimeout);

  m_timer.clear();
  m_ui->copyButton->setText(tr("Copy"));
  ThemeIcon::setIcon(*m_ui->copyButton, ":/icons/copy.svg");
}

void SettingsWidget::homepageClicked()
{
  QDesktopServices::openUrl(QUrl("https://github.com/KoromeloDev/MindMate"));
}

void SettingsWidget::reportClicked()
{
  QDesktopServices::openUrl(QUrl(
  "https://github.com/KoromeloDev/MindMate/issues/new?template=bug_report.md"));
}

void SettingsWidget::updateColor()
{
  QColor uColor1 = m_ui->uColorEdit1->text();
  QColor uColor2 = m_ui->uColorEdit2->text();
  QColor aColor1 = m_ui->aColorEdit1->text();
  QColor aColor2 = m_ui->aColorEdit2->text();
  QColor sColor1 = m_ui->sColorEdit1->text();
  QColor sColor2 = m_ui->sColorEdit2->text();

  if (uColor1.isValid())
  {
    m_ui->uColor1->setStyleSheet("background-color: " + m_ui->uColorEdit1->text());
  }

  if (uColor2.isValid())
  {
    m_ui->uColor2->setStyleSheet("background-color: " + m_ui->uColorEdit2->text());
  }

  if (aColor1.isValid())
  {
    m_ui->aColor1->setStyleSheet("background-color: " + m_ui->aColorEdit1->text());
  }

  if (aColor2.isValid())
  {
    m_ui->aColor2->setStyleSheet("background-color: " + m_ui->aColorEdit2->text());
  }

  if (sColor1.isValid())
  {
    m_ui->sColor1->setStyleSheet("background-color: " + m_ui->sColorEdit1->text());
  }

  if (sColor2.isValid())
  {
    m_ui->sColor2->setStyleSheet("background-color: " + m_ui->sColorEdit2->text());
  }

}

void SettingsWidget::writeColorSettings()
{
  QColor uColor1 = m_ui->uColorEdit1->text();
  QColor uColor2 = m_ui->uColorEdit2->text();
  QColor aColor1 = m_ui->aColorEdit1->text();
  QColor aColor2 = m_ui->aColorEdit2->text();
  QColor sColor1 = m_ui->sColorEdit1->text();
  QColor sColor2 = m_ui->sColorEdit2->text();

  if (uColor1.isValid())
  {
    m_settings.userMessageColor1 = m_ui->uColorEdit1->text();
  }

  if (uColor2.isValid())
  {
    m_settings.userMessageColor2 = m_ui->uColorEdit2->text();
  }

  if (aColor1.isValid())
  {
    m_settings.assistantMessageColor1 = m_ui->aColorEdit1->text();
  }

  if (aColor2.isValid())
  {
    m_settings.assistantMessageColor2 = m_ui->aColorEdit2->text();
  }

  if (sColor1.isValid())
  {
    m_settings.systemMessageColor1 = m_ui->sColorEdit1->text();
  }

  if (sColor2.isValid())
  {
    m_settings.systemMessageColor2 = m_ui->sColorEdit2->text();
  }

  m_settings.writeSettings();
}
