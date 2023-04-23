#include "SettingsWidget.h"
#include "ui_SettingsWidget.h"

#include "EditDialog.h"
#include "ThemeIcon.h"

SettingsWidget::SettingsWidget(QWidget *parent)
: QDialog(parent), m_ui(new Ui::SettingsWidget)
{
	m_ui->setupUi(this);
	QString version = QApplication::applicationVersion();
	m_ui->version->setText(tr("Version") + ": " + version);
	m_ui->version->setAlignment(Qt::AlignCenter);

	connect(m_ui->editKeyButton, &QToolButton::clicked,
	        this, &SettingsWidget::editKeyClicked);

	readSettings();
	showSettings();
	ThemeIcon::setIcon(*m_ui->editKeyButton, ":/icons/edit.svg");
}

SettingsWidget::~SettingsWidget()
{
	delete m_ui;
}

QString SettingsWidget::getOpenAIKey()
{
	readSettings();
	return m_openAIKey;
}

void SettingsWidget::writeSettings()
{
	QSettings settings("settings.ini", QSettings::IniFormat);
	settings.beginGroup("Settings");
	settings.setValue("OpenAI_Key", m_openAIKey);
	settings.endGroup();
}

void SettingsWidget::readSettings()
{
	QSettings settings("settings.ini", QSettings::IniFormat);
	settings.beginGroup("Settings");
	m_openAIKey = settings.value("OpenAI_Key").toString();
	settings.endGroup();
}

void SettingsWidget::showSettings()
{
	if (m_openAIKey.left(3) == "sk-" && m_openAIKey.length() == 51)
	{
		QString keyHidden = m_openAIKey.left(3) + "..." + m_openAIKey.right(4);
		m_ui->keyLabel->setText(tr("API key") + ": " + keyHidden);
		m_ui->statusLabel->setPixmap(QPixmap(":/icons/dot_green.svg"));
	}
	else
	{
		m_ui->statusLabel->setPixmap(QPixmap(":/icons/dot_red.svg"));
	}
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
			writeSettings();
			showSettings();
		}
		else
		{
			QMessageBox::information(this, tr("Wrong key"),
			                         tr("You entered the wrong key"));
		}
	});

	dialog->show();
}
