#include "CodeWidget.h"
#include "ui_CodeWidget.h"

#include "ThemeIcon.h"

CodeWidget::CodeWidget(QWidget *parent, QString code, QMenu *menu)
: QWidget(parent), m_ui(new Ui::CodeWidget)
{
	m_ui->setupUi(this);
	quint8 i = 0;

	while (i < code.length())
	{
		if (code[i] == '\n' || code[i] == ' ' ||
				code[i] == '\t' || code[i].isNull())
		{
			break;
		}

		m_language.append(code[i]);
		i++;
	}

	quint8 j = 0;

	while (true)
	{
		QChar symbol = code[code.length() -1 - j];

		if (symbol == '\n' || symbol == ' ' || symbol == '\t' || symbol.isNull())
		{
			j++;
		}
		else
		{
			break;
		}
	}

	code.chop(j);
	m_code = code;
	m_code.remove(0, i + 1);
	m_ui->languageLabel->setText(m_language);
	m_ui->codeBrowser->setText(m_code);
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
}

CodeWidget::~CodeWidget()
{
	delete m_ui;
}

void CodeWidget::resizeWidget()
{
	quint16 sizeWidth = parentWidget()->size().width();
	quint16 sizeHeight = m_ui->codeBrowser->document()->size().toSize().height();
	setMaximumWidth(sizeWidth);
	setMaximumHeight(sizeHeight + 70);
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
