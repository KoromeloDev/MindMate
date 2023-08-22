#include "ChatSettingsWidget.h"
#include "ui_ChatSettingsWidget.h"

#include "ThemeIcon.h"

ChatSettingsWidget::ChatSettingsWidget(QWidget *parent, ChatSettings settings)
: QWidget(parent), m_ui(new Ui::ChatSettingsWidget)
{
  m_ui->setupUi(this);

  ThemeIcon::setIcon(*m_ui->addStopButton, ":/resources/icons/add.svg");
  ThemeIcon::setIcon(*m_ui->deleteStopButton, ":/resources/icons/delete.svg");

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
  connect(m_ui->addStopButton, &QToolButton::clicked,
          this, &ChatSettingsWidget::addClicked);
  connect(m_ui->deleteStopButton, &QToolButton::clicked,
          this, &ChatSettingsWidget::deleteClicked);

  m_settings = settings;
  setParameters();

  if (m_settings.stop.length() == 0)
  {
    m_ui->deleteStopButton->setEnabled(false);
  }
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

  m_ui->stopList->addItems(m_settings.stop);
  m_ui->stopList->setCurrentRow(m_ui->stopList->count() - 1);
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

void ChatSettingsWidget::addClicked()
{
  m_editDialog = m_editDialog.create(this);
  m_editDialog->setWindowTitle(tr("Add stop word"));

  connect(m_editDialog.get(), &EditDialog::textChanged,
          this, &ChatSettingsWidget::addStopWord);

  m_editDialog->show();
}

void ChatSettingsWidget::addStopWord(const QString text)
{
  m_settings.stop.append(text);
  quint8 count = m_settings.stop.length();

  if (count == 4)
  {
    m_ui->addStopButton->setEnabled(false);
  }

  if (count == 1)
  {
    m_ui->deleteStopButton->setEnabled(true);
  }

  m_ui->stopList->addItem(text);
  m_ui->stopList->setCurrentRow(m_ui->stopList->count() - 1);
}

void ChatSettingsWidget::deleteClicked()
{
  quint8 index = m_ui->stopList->currentRow();
  m_ui->stopList->takeItem(index);
  m_settings.stop.removeAt(index);
  quint8 count = m_settings.stop.length();

  if (count < 4)
  {
    m_ui->addStopButton->setEnabled(true);
  }

  if (count == 0)
  {
    m_ui->deleteStopButton->setEnabled(false);
  }
}
