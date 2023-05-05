#include "SettingsWidget.h"
#include "ui_SettingsWidget.h"

#include "EditDialog.h"
#include "ThemeIcon.h"

SettingsWidget::SettingsWidget(QWidget *parent)
: QDialog(parent), m_ui(new Ui::SettingsWidget)
{
	m_ui->setupUi(this);
	m_ui->verticalLayout_2->addWidget(&m_chatSettingsWidget);
	QString version = QApplication::applicationVersion();
	m_ui->version->setText(tr("Version") + ": " + version);
	m_ui->version->setAlignment(Qt::AlignCenter);

	connect(m_ui->editKeyButton, &QToolButton::clicked,
					this, &SettingsWidget::editKeyClicked);
	connect(m_ui->checkUpdates, &QCheckBox::stateChanged,
					this, &SettingsWidget::checkUpdatesStateChanged);

	readSettings();
	showSettings();
	ThemeIcon::setIcon(*m_ui->editKeyButton, ":/icons/edit.svg");
}

SettingsWidget::~SettingsWidget()
{
	delete m_ui;
}

const QString SettingsWidget::getOpenAIKey()
{
	readSettings();
	return m_openAIKey;
}

const bool SettingsWidget::getCheckUpdates()
{
	readSettings();
	return m_checkUpdates;
}

const bool SettingsWidget::getLanguageRecognize()
{
	readSettings();
	return m_languageRecognize;
}

const ChatSettings SettingsWidget::getChatSettings()
{
	readSettings();
	return m_chatSettings;
}

void SettingsWidget::closeEvent(QCloseEvent *event)
{
	m_chatSettings = m_chatSettingsWidget.getSettings();
	writeSettings();
	QWidget::closeEvent(event);
}

void SettingsWidget::writeSettings()
{
	QSettings settings("settings.ini", QSettings::IniFormat);
	settings.beginGroup("Main");
	settings.setValue("CheckUpdates", m_checkUpdates);
	settings.setValue("AutoLanguageRecognize", m_languageRecognize);
	settings.setValue("AutoNamingChat", m_autoNaming);
	settings.endGroup();

	settings.beginGroup("Chat");
	settings.setValue("OpenAIKey", m_openAIKey);
	settings.setValue("Model", m_chatSettings.model);
	settings.setValue("Temperature", m_chatSettings.temperature);
	settings.setValue("N", m_chatSettings.n);

	for (quint8 i = 0; i < m_chatSettings.stop.length(); i++)
	{
		settings.setValue("StopWord" + QString::number(i), m_chatSettings.stop[i]);
	}

	settings.setValue("PresencePenalty", m_chatSettings.presencePenalty);
	settings.setValue("FrequencyPenalty", m_chatSettings.frequencyPenalty);
	settings.endGroup();
}

void SettingsWidget::readSettings()
{
	QSettings settings("settings.ini", QSettings::IniFormat);
	settings.beginGroup("Main");
	m_checkUpdates = settings.value("CheckUpdates").toBool();

	if (!settings.contains("AutoLanguageRecognize"))
	{
		settings.setValue("AutoLanguageRecognize", true);
	}

	if (!settings.contains("AutoLanguageRecognize"))
	{
		settings.setValue("AutoNamingChat", true);
	}

	m_languageRecognize = settings.value("AutoLanguageRecognize").toBool();
	m_autoNaming = settings.value("AutoLanguageRecognize").toBool();
	settings.endGroup();

	settings.beginGroup("Chat");
	m_openAIKey = settings.value("OpenAIKey").toString();

	QString model = settings.value("Model").toString();

	if (!model.isEmpty())
	{
		m_chatSettings.model = model;
	}

	if (!settings.value("Temperature").isNull())
	{
		m_chatSettings.temperature = settings.value("Temperature").toFloat();
	}

	if (!settings.value("N").isNull())
	{
		m_chatSettings.n = settings.value("N").toUInt();
	}

	QStringList stopList;

	for (quint8 i = 0; i < 4; i++)
	{
		QString stopWord = settings.value("StopWord" + QString::number(i)).
											 toString();
		if (!stopWord.isEmpty())
		{
			stopList.append(stopWord);
		}
	}

	if (stopList.length() !=0)
	{
		m_chatSettings.stop = stopList;
	}

	if (!settings.value("PresencePenalty").isNull())
	{
		m_chatSettings.presencePenalty = settings.value("PresencePenalty").
																		 toFloat();
	}

	if (!settings.value("FrequencyPenalty").isNull())
	{
		m_chatSettings.frequencyPenalty = settings.value("FrequencyPenalty").
																			toFloat();
	}

	settings.endGroup();
}

void SettingsWidget::showSettings()
{
	m_ui->checkUpdates->setChecked(m_checkUpdates);
	m_ui->langRecognize->setChecked(m_languageRecognize);
	m_ui->autoNamingChat->setChecked(m_autoNaming);

	if (m_openAIKey.left(3) == "sk-" && m_openAIKey.length() == 51)
	{
		QString keyHidden = m_openAIKey.left(3) + "..." + m_openAIKey.right(4);
		m_ui->keyLabel->setText(tr("API key") + ": " + keyHidden);
	}

	m_chatSettingsWidget.setSettings(m_chatSettings);
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
			m_openAIKey = key;
			showSettings();
		}
		else
		{
			QMessageBox::critical(this, tr("Wrong key"),
														tr("You entered the wrong key"));
		}
	});

	dialog->show();
}

void SettingsWidget::checkUpdatesStateChanged(quint8 state)
{
	m_checkUpdates = state;
}
