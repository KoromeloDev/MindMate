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
  m_ui->copyButton->setText(tr("Copy info"));
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
