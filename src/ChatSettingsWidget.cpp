#include "ChatSettingsWidget.h"
#include "ui_ChatSettingsWidget.h"

#include "ThemeIcon.h"

ChatSettingsWidget::ChatSettingsWidget(QWidget *parent, ChatSettings settings)
: QWidget(parent), m_ui(new Ui::ChatSettingsWidget)
{
  m_ui->setupUi(this);
  connect(m_ui->modelSelector, &QComboBox::currentTextChanged,
          this, &ChatSettingsWidget::modelChanged);
  connect(m_ui->temperatureSlider, &QSlider::sliderMoved,
          this, &ChatSettingsWidget::temperatureSliderMoved);
  connect(m_ui->nSlider, &QSlider::sliderMoved,
          this, &ChatSettingsWidget::nSliderMoved);
  connect(m_ui->pPSlider, &QSlider::sliderMoved,
          this, &ChatSettingsWidget::pPSliderMoved);
  connect(m_ui->fPSlider, &QSlider::sliderMoved,
          this, &ChatSettingsWidget::fPSliderMoved);

  ThemeIcon::setIcon(*m_ui->addStopButton, ":/resources/icons/add.svg");
  ThemeIcon::setIcon(*m_ui->deleteStopButton, ":/resources/icons/delete.svg");

  m_settings = settings;
  setParameters();
}

ChatSettingsWidget::~ChatSettingsWidget()
{
  delete m_ui;
}

ChatSettings ChatSettingsWidget::getSettings() const
{
  return m_settings;
}

void ChatSettingsWidget::changeEvent(QEvent *event)
{
  QWidget::changeEvent(event);

  switch (event->type())
  {
    case QEvent::LanguageChange:
      m_ui->retranslateUi(this);
      break;
    default:
      break;
  }
}

void ChatSettingsWidget::setParameters()
{
  m_ui->modelSelector->setCurrentText(m_settings.model);

  m_ui->temperatureSlider->setValue(m_settings.temperature*10);
  m_ui->temperatureValue->setText(QString::number(m_settings.temperature));

  m_ui->nSlider->setValue(m_settings.n);
  m_ui->nValue->setText(QString::number(m_settings.n));

  m_ui->pPSlider->setValue(m_settings.presencePenalty*10);
  m_ui->pPValue->setText(QString::number(m_settings.presencePenalty));

  m_ui->fPSlider->setValue(m_settings.frequencyPenalty*10);
  m_ui->fPValue->setText(QString::number(m_settings.frequencyPenalty));
}

void ChatSettingsWidget::modelChanged(const QString &text)
{
  m_settings.model = text;
}

void ChatSettingsWidget::temperatureSliderMoved(quint8 position)
{
  m_settings.temperature = (float)position / 10;
  m_ui->temperatureValue->setText(QString::number(m_settings.temperature));
}

void ChatSettingsWidget::nSliderMoved(quint8 position)
{
  m_settings.n = position;
  m_ui->nValue->setText(QString::number(position));
}

void ChatSettingsWidget::pPSliderMoved(qint8 position)
{
  m_settings.presencePenalty = (float)position / 10;
  m_ui->pPValue->setText(QString::number((float)position/10));
}

void ChatSettingsWidget::fPSliderMoved(qint8 position)
{
  m_settings.frequencyPenalty = (float)position / 10;
  m_ui->fPValue->setText(QString::number((float)position/10));
}
