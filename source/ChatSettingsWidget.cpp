#include "ChatSettingsWidget.h"
#include "ui_ChatSettingsWidget.h"

#include "ThemeIcon.h"

ChatSettingsWidget::ChatSettingsWidget(QWidget *parent, quint8 index)
: QDialog(parent), m_ui(new Ui::ChatSettingsWidget)
{
	m_ui->setupUi(this);
	m_index = index;

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
	connect(m_ui->buttonBox, &QDialogButtonBox::accepted,
					this, &QDialog::accept);
	connect(m_ui->buttonBox, &QDialogButtonBox::rejected,
					this, &QDialog::reject);

	ThemeIcon::setIcon(*m_ui->addStopButton, ":/icons/add.svg");
	ThemeIcon::setIcon(*m_ui->deleteStopButton, ":/icons/delete.svg");

	m_chatSettings = ChatSettings::getSettings(m_index);
	setParameters();
}

ChatSettingsWidget::~ChatSettingsWidget()
{
	delete m_ui;
}

void ChatSettingsWidget::setParameters()
{
	m_ui->modelSelector->setCurrentText(m_chatSettings.model);

	m_ui->temperatureSlider->setValue(m_chatSettings.temperature*10);
	m_ui->temperatureValue->setText(QString::number(m_chatSettings.temperature));

	m_ui->nSlider->setValue(m_chatSettings.n);
	m_ui->nValue->setText(QString::number(m_chatSettings.n));

	m_ui->pPSlider->setValue(m_chatSettings.presencePenalty*10);
	m_ui->pPValue->setText(QString::number(m_chatSettings.presencePenalty));

	m_ui->fPSlider->setValue(m_chatSettings.frequencyPenalty*10);
	m_ui->fPValue->setText(QString::number(m_chatSettings.frequencyPenalty));
}

void ChatSettingsWidget::accept()
{
	QFile chatJson(QDir::currentPath() + "/chats.json");

	if (chatJson.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QByteArray jsonData = chatJson.readAll();
		chatJson.close();
		QJsonDocument document = QJsonDocument::fromJson(jsonData);
		QJsonArray chats = document.array();

		if (chats.isEmpty())
		{
			return;
		}

		if (chatJson.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QJsonObject object = chats[m_index].toObject();
			object["model"] = m_chatSettings.model;
			object["temperature"] = m_chatSettings.temperature;
			object["n"] = m_chatSettings.n;
			object["presence_penalty"] = m_chatSettings.presencePenalty;
			object["frequency_penalty"] = m_chatSettings.frequencyPenalty;
			chats[m_index] = object;
			document.setArray(chats);
			chatJson.write(document.toJson());
			chatJson.close();
		}
	}

	QDialog::accept();
}

void ChatSettingsWidget::modelChanged(QString text)
{
	m_chatSettings.model = text;
}

void ChatSettingsWidget::temperatureSliderMoved(quint8 position)
{
	m_chatSettings.temperature = (float)position / 10;
	m_ui->temperatureValue->setText(QString::number(m_chatSettings.temperature));
}

void ChatSettingsWidget::nSliderMoved(quint8 position)
{
	m_chatSettings.n = position;
	m_ui->nValue->setText(QString::number(position));
}

void ChatSettingsWidget::pPSliderMoved(qint8 position)
{
	m_chatSettings.presencePenalty = (float)position / 10;
	m_ui->pPValue->setText(QString::number((float)position/10));
}

void ChatSettingsWidget::fPSliderMoved(qint8 position)
{
	m_chatSettings.frequencyPenalty = (float)position / 10;
	m_ui->fPValue->setText(QString::number((float)position/10));
}
