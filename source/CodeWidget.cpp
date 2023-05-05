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

QString CodeWidget::getCode()
{
	return m_code;
}

void CodeWidget::setCodeAutoHighlighter()
{
	using QSH = QSourceHighlite::QSourceHighliter;
	using QSHL = QSourceHighlite::QSourceHighliter::Language;
	QSH::Language languageEnum;
	QString language = m_language.toLower();
	SettingsWidget settings;

	if (language.isEmpty() && settings.getLanguageRecognize())
	{
		OpenAIChat *chatGPT = new OpenAIChat(this, settings.getOpenAIKey());

		connect(chatGPT, &OpenAIChat::responseReceived,
						this, [=]()
		{
			m_language = chatGPT->getAnswerMessage().content;

			if (!m_language.isEmpty())
			{
				m_language = m_language.left(m_language.indexOf(" "));
				m_language = m_language.left(m_language.indexOf("."));
				m_language = m_language.toLower();
				m_ui->languageLabel->setText(m_language);
				emit changeLanguage(m_language);
				setCodeAutoHighlighter();
			}
		});

		chatGPT->chat("Write in one word what kind of programming language:\n"
									+ m_code);
	}
	else if (language.contains("assembly"))
	{
		languageEnum = QSHL::CodeAsm;
	}
	else if (language.contains("bash"))
	{
		languageEnum = QSHL::CodeBash;
	}
	else if (language.contains("c"))
	{
		if (language.contains("cmake"))
		{
			languageEnum = QSHL::CodeCMake;
		}
		else if (language.contains("css"))
		{
			languageEnum = QSHL::CodeCSS;
		}
		else if (language.contains("c#"))
		{
			languageEnum = QSHL::CodeCSharp;
		}
		else if (language.contains("cpp") || language.contains("c++"))
		{
			languageEnum = QSHL::CodeCpp;
		}
		else
		{
			languageEnum = QSHL::CodeC;
		}
	}
	else if (language.contains("go"))
	{
		languageEnum = QSHL::CodeGo;
	}
	else if (language.contains("ini"))
	{
		languageEnum = QSHL::CodeINI;
	}
	else if (language.contains("json"))
	{
		languageEnum = QSHL::CodeJSON;
	}
	else if (language.contains("javascript") || language.contains("js"))
	{
		languageEnum = QSHL::CodeJs;
	}
	else if (language.contains("java"))
	{
		languageEnum = QSHL::CodeJava;
	}
	else if (language.contains("lua"))
	{
		languageEnum = QSHL::CodeLua;
	}
	else if (language.contains("php"))
	{
		languageEnum = QSHL::CodePHP;
	}
	else if (language.contains("python"))
	{
		languageEnum = QSHL::CodePython;
	}
	else if (language.contains("qml"))
	{
		languageEnum = QSHL::CodeQML;
	}
	else if (language.contains("rust"))
	{
		languageEnum = QSHL::CodeRust;
	}
	else if (language.contains("sql"))
	{
		languageEnum = QSHL::CodeSQL;
	}
	else if (language.contains("typescript"))
	{
		languageEnum = QSHL::CodeTypeScript;
	}
	else if (language.contains("v"))
	{
		if (language.contains("vex"))
		{
			languageEnum = QSHL::CodeVex;
		}
		else
		{
			languageEnum = QSHL::CodeV;
		}
	}
	else if (language.contains("xml"))
	{
		languageEnum = QSHL::CodeXML;
	}
	else if (language.contains("yaml"))
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
