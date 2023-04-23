#include "NewTextEdit.h"
#include "ui_NewTextEdit.h"

NewTextEdit::NewTextEdit(QWidget *parent)
: QTextEdit(parent), m_ui(new Ui::NewTextEdit)
{
	m_ui->setupUi(this);
	connect(this, &QTextEdit::textChanged, this, &NewTextEdit::resizeTextInput);
	setAcceptRichText(false);
	setFocus();
	verticalScrollBar()->setSingleStep(5);
}

NewTextEdit::~NewTextEdit()
{
	delete m_ui;
}

void NewTextEdit::resizeTextInput()
{
	quint16 sizeHeight = document()->size().toSize().height();
	quint16 lines = sizeHeight / 20;

	if (lines <= 5)
	{
		setMaximumHeight(sizeHeight + 4);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}
	else
	{
		setMaximumHeight(120);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	}
}

void NewTextEdit::keyPressEvent(QKeyEvent *event)
{
	if (event->modifiers() != Qt::ShiftModifier &&
	    (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter))
	{
		QString text = toPlainText();

		quint16 i = 0;

		for (QChar symbol : text)
		{
			if (symbol == '\n' || symbol == ' ' || symbol == '\t' || symbol.isNull())
			{
				i++;
			}
			else
			{
				break;
			}
		}

		text = text.last(text.length() - i);
		i = 0;

		while(true)
		{
			quint16 index = text.length() -1 - i;

			if (index >= text.length())
			{
				break;
			}

			QChar symbol = text[index];

			if (symbol == '\n' || symbol == ' ' || symbol == '\t' || symbol.isNull())
			{
				i++;
			}
			else
			{
				break;
			}
		}

		text = text.first(text.length() - i);
		emit sendText(text);
	}
	else
	{
		QTextEdit::keyPressEvent(event);
	}
}

void NewTextEdit::resizeEvent(QResizeEvent *event)
{
	QTextEdit::resizeEvent(event);
	resizeTextInput();
}
