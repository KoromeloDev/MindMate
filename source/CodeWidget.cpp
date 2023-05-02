#include "CodeWidget.h"
#include "ui_CodeWidget.h"

#include "ThemeIcon.h"
#include "OpenAIChat.h"
#include "SettingsWidget.h"

CodeWidget::CodeWidget(QWidget *parent, QString code, QMenu *menu)
: QWidget(parent), m_ui(new Ui::CodeWidget)
{
	m_ui->setupUi(this);
	quint8 i = 0;
	m_code = code;

	while (i < code.length())
	{
		if (code[i] == '\n' || code[i] == ' ' ||
				code[i] == '\t' || code[i].isNull())
		{
			break;
		}

		i++;
	}

	m_language.append(code.mid(3, i - 3));
	quint8 j = 0;

	while (code.length() != 0)
	{
		QChar symbol = code[code.length() -1 - j];

		if (symbol == '`')
		{
			j++;
		}
		else
		{
			break;
		}
	}

	m_code.chop(j + 1);
	m_code.remove(0, i + 1);
	m_ui->languageLabel->setText(m_language);
	m_ui->codeBrowser->setPlainText(m_code);
	m_clipboard = QApplication::clipboard();
	m_ui->codeBrowser->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(m_ui->copyButton, &QToolButton::clicked,
					this, &CodeWidget::copyClicked);
	connect(m_ui->codeBrowser, &QTextBrowser::customContextMenuRequested,
					this, [=]()
	{
		menu->exec(QCursor::pos());
	});

	ThemeIcon::setIcon(*m_ui->copyButton, ":/icons/copy.svg");
	setCodeAutoHighlighter();
}

CodeWidget::~CodeWidget()
{
	delete m_ui;
	delete m_highlighter;
}

void CodeWidget::resizeWidget(quint16 margin)
{
	quint16 sizeWidth = parentWidget()->size().width() - 30 - margin;
	quint16 sizeHeight = m_ui->codeBrowser->document()->size().toSize().height() +
											 70;
	m_ui->codeBrowser->setMaximumWidth(sizeWidth);
	m_ui->codeBrowser->setMaximumHeight(sizeHeight);
	setMaximumWidth(sizeWidth + 40);
	setMaximumHeight(sizeHeight);
}

void CodeWidget::setCodeAutoHighlighter()
{
	using QSH = QSourceHighlite::QSourceHighliter;
	using QSHL = QSourceHighlite::QSourceHighliter::Language;
	QSH::Language languageEnum;
	QString language = m_language.toLower();

	if (language.isEmpty())
	{
		SettingsWidget settings;
		OpenAIChat *chatGPT = new OpenAIChat(this, settings.getOpenAIKey());

		connect(chatGPT, &OpenAIChat::responseReceived,
						this, [=]()
		{
			m_language = chatGPT->getAnswerMessage().content;
			m_ui->languageLabel->setText(m_language);
			setCodeAutoHighlighter();
		});

		chatGPT->chat("Write in one word what kind of programming language:\n"
									+ m_code);
	}
	else if (language == "assembly")
	{
		languageEnum = QSHL::CodeAsm;
	}
	else if (language == "bash")
	{
		languageEnum = QSHL::CodeBash;
	}
	else if (language == "c")
	{
		languageEnum = QSHL::CodeC;
	}
	else if (language == "cmake")
	{
		languageEnum = QSHL::CodeCMake;
	}
	else if (language == "css")
	{
		languageEnum = QSHL::CodeCSS;
	}
	else if (language == "c#")
	{
		languageEnum = QSHL::CodeCSharp;
	}
	else if (language == "cpp")
	{
		languageEnum = QSHL::CodeCpp;
	}
	else if (language == "c++")
	{
		languageEnum = QSHL::CodeCpp;
	}
	else if (language == "go")
	{
		languageEnum = QSHL::CodeGo;
	}
	else if (language == "ini")
	{
		languageEnum = QSHL::CodeINI;
	}
	else if (language == "json")
	{
		languageEnum = QSHL::CodeJSON;
	}
	else if (language == "java")
	{
		languageEnum = QSHL::CodeJava;
	}
	else if (language == "js")
	{
		languageEnum = QSHL::CodeJs;
	}
	else if (language == "javascript")
	{
		languageEnum = QSHL::CodeJs;
	}
	else if (language == "lua")
	{
		languageEnum = QSHL::CodeLua;
	}
	else if (language == "php")
	{
		languageEnum = QSHL::CodePHP;
	}
	else if (language == "python")
	{
		languageEnum = QSHL::CodePython;
	}
	else if (language == "qml")
	{
		languageEnum = QSHL::CodeQML;
	}
	else if (language == "rust")
	{
		languageEnum = QSHL::CodeRust;
	}
	else if (language == "sql")
	{
		languageEnum = QSHL::CodeSQL;
	}
	else if (language == "typescript")
	{
		languageEnum = QSHL::CodeTypeScript;
	}
	else if (language == "v")
	{
		languageEnum = QSHL::CodeV;
	}
	else if (language == "vex")
	{
		languageEnum = QSHL::CodeVex;
	}
	else if (language == "xml")
	{
		languageEnum = QSHL::CodeXML;
	}
	else if (language == "yaml")
	{
		languageEnum = QSHL::CodeYAML;
	}
	else
	{
		languageEnum = QSHL::CodeBash;
	}

	QTextDocument *document = m_ui->codeBrowser->document();
	m_highlighter = new QSH(document);
	m_highlighter->setCurrentLanguage(languageEnum);
}

void CodeWidget::copyClicked()
{
	m_clipboard->setText(m_code);
	m_ui->copyButton->setText(tr("Copied") + "!");
	ThemeIcon::setIcon(*m_ui->copyButton, ":/icons/accept.svg");

	QTimer *timer = new QTimer(this);
	timer->setSingleShot(true);
	timer->setInterval(1500);
	timer->start();

	connect(timer, &QTimer::timeout, this, [=]()
	{
		timer->deleteLater();
		m_ui->copyButton->setText(tr("Copy"));
		ThemeIcon::setIcon(*m_ui->copyButton, ":/icons/copy.svg");
	});
}
