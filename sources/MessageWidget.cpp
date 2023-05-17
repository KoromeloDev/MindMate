#include "MessageWidget.h"
#include "ui_MessageWidget.h"

#include "ThemeIcon.h"
#include "ChatSettings.h"

MessageWidget::MessageWidget(QListWidgetItem *item,
														 HistoryParser::Message message, quint8 chatIndex,
														 quint8 index) : m_ui(new Ui::MessageWidget)
{
	m_ui->setupUi(this);
	m_item = item;
	m_message = message;
	m_chatIndex = chatIndex;
	m_index = index;
	QString colorOne;
	QString colorTwo;

	switch (message.role)
	{
		case HistoryParser::Role::Assistant:
			  colorOne = "ef745c";
				colorTwo = "6E3AD5";
		    break;

		case HistoryParser::Role::User:
			  colorOne = "40c9ff";
				colorTwo = "e81cff";
		    break;

		case HistoryParser::Role::System:
			  colorOne = "e20b8c";
				colorTwo = "f84b00";
		    break;
	}

	setStyleSheet("background: qlineargradient("
								"x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #" + colorOne +", "
								"stop: 1 #" + colorTwo +");");
	m_menu = new QMenu(this);
	m_menu->setStyleSheet("QMenu {color: white;border: 1px solid white;}"
												"QMenu::item {background-color: black;}"
												"QMenu::item:selected {background-color: #201E1C;}");
	m_menu->addAction(ThemeIcon::getIcon(":/icons/delete.svg"), tr("Delete"),
										this, &MessageWidget::actionDeleteClicked);
	createText();
	selection("`([^`]*)`");
	selection("'([^']*)'");
	selection("\"([^\"]*)\"");

	if (index == 0)
	{
		m_timer = new QTimer(this);
		m_timer->setSingleShot(true);
		m_timer->setInterval(50);

		connect(m_timer, &QTimer::timeout, this, [=]()
		{
			m_timer->deleteLater();
			m_timer = nullptr;
			resize();
		});

		m_timer->start();
	}
}

MessageWidget::~MessageWidget()
{
	delete m_ui;
	m_layout->deleteLater();
	delete m_menu;

	if (m_timer != nullptr)
	{
		m_timer->deleteLater();
	}

	for (quint8 i = 0; i < m_codeWidgets.count(); i++)
	{
		delete m_codeWidgets[i];
	}

	for (quint8 i = 0; i < m_textBrowsers.count(); i++)
	{
		delete m_textBrowsers[i];
	}
}

void MessageWidget::resize()
{
	m_height = 0;
	m_width = 0;
	quint8 i = 0;

	if (m_textBrowsers.length() != 0)
	{
		for (QTextBrowser *textBrowser : qAsConst(m_textBrowsers))
		{
			QSize textParameter = getSizeTextBrowser(textBrowser, i);
			m_height += textParameter.height();

			if (textParameter.width() > m_width)
			{
				m_width = textParameter.width();
			}

			i++;
		}
	}
	else
	{
		m_width = parentWidget()->size().width() * 0.7;

		if (m_message.role == HistoryParser::Role::User)
		{
			quint16 parentSize = parentWidget()->size().width();
			m_leftMargin = parentSize - m_width;
			parentSize += m_leftMargin;
			m_width += m_leftMargin;
			setContentsMargins(m_leftMargin, 0, 10, 0);
		}
		else if (m_message.role == HistoryParser::Role::System)
		{
			m_width = parentWidget()->size().width();
		}
	}

	for (CodeWidget *code : qAsConst(m_codeWidgets))
	{
		m_height += code->size().height();
	}

	setFixedWidth(m_width);
	setMinimumHeight(m_height);

	if (m_item != nullptr)
	{
		m_item->setSizeHint(QSize(width(), m_height+12));
	}

	for (CodeWidget *code : qAsConst(m_codeWidgets))
	{
		code->resizeWidget(m_leftMargin);
	}

	emit resizeFinished();
}

QSize MessageWidget::getSizeTextBrowser(QTextBrowser *textBrowser, quint8 index)
{
	quint16 maxSizeWidth = parentWidget()->size().width();
	float sizeWidth = 32;

	if (m_message.role != HistoryParser::Role::System)
	{
		maxSizeWidth *= 0.7;

		if (isMaxWidth())
		{
			sizeWidth = maxSizeWidth;
		}
		else if (m_textWidth.length() != 0)
		{
			sizeWidth += m_textWidth[index];
		}
	}
	else
	{
		sizeWidth = maxSizeWidth - 42;
		textBrowser->setAlignment(Qt::AlignCenter);
	}

	if (m_message.role == HistoryParser::Role::User)
	{
		quint16 margin = parentWidget()->size().width() - sizeWidth - 31;
		m_leftMargin = margin;
		sizeWidth += margin;
		setContentsMargins(margin, 0, 10, 0);
	}

	quint16 sizeHeight = textBrowser->document()->size().toSize().height() + 12;
	sizeWidth += 40;
	textBrowser->setMaximumWidth(sizeWidth);
	textBrowser->setMaximumHeight(sizeHeight);

	return QSize(sizeWidth, sizeHeight);
}

void MessageWidget::createText()
{
	m_layout = new QVBoxLayout(this);
	m_layout->setSpacing(0);
	m_layout->setContentsMargins(10, 0, 10, 0);
	setLayout(m_layout);

	static QRegularExpression re(
	"\n* *\t*(`{3}([\\w\\+\\#\\-\\.]*\n[\\s\\S]*?)\t* *\n*`{3})\t* *\n*");
	QRegularExpressionMatchIterator matchIterator =
																	re.globalMatch(m_message.content);
	QList<qint16> positions = {0};
	QList<qint16> lenght =  {0};
	QString text = m_message.content;
	QStringList codeText;
	quint8 indexCode = 0;
	QStringList textList;

	while (matchIterator.hasNext())
	{
		QRegularExpressionMatch match = matchIterator.next();

		if (positions.count() == 1)
		{
			positions.append(match.capturedStart());
		}

		lenght.append(match.capturedLength());
		positions.append(match.capturedEnd());
		codeText.append(match.captured(1));
	}

	positions.append(text.length());
	lenght.append(-1);

	for (quint8 i = 0; i < lenght.count(); i++)
	{
		if (i + 1 != positions.count())
		{
			quint16 n = positions[i+1] - lenght[i] - positions[i];
			QString subText = text.mid(positions[i], n);

			if (!subText.isEmpty())
			{
				textList.append(subText);
			}
		}
	}

	for (const QString &subText : textList)
	{
		QTextBrowser *textBrowser = new QTextBrowser(this);
		Border border;
		textBrowser->setStyleSheet("padding-left:10px;"
															 "padding-right:10px;"
															 "padding-top:5px;"
															 "padding-bottom:5px;");
		textBrowser->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textBrowser->setMinimumSize(40, 40);
		textBrowser->setFont(QFont(":/fonts/Roboto-Regular.ttf"));
		m_layout->addWidget(textBrowser);
		m_textBrowsers.append(textBrowser);
		textBrowser->setText(subText);
		textBrowser->setContextMenuPolicy(Qt::CustomContextMenu);
		QFontMetrics fontSize(textBrowser->font());
		m_textWidth.append(fontSize.boundingRect(QRect(), Qt::TextDontClip, text).
											 width());

		connect(textBrowser, &QTextBrowser::customContextMenuRequested, this, [=]()
		{
			m_menu->exec(QCursor::pos());
		});

		if (subText == textList[0])
		{
			border.topLeft = 15;
			border.topRight = 15;
		}

		if (subText == textList.last() && indexCode + 1 != codeText.count())
		{
			border.bottomLeft = 15;
			border.bottomRight = 15;
		}

		setBorder(textBrowser, border);

		if (indexCode < codeText.count())
		{
			Border border;

			if (subText == textList.last())
			{
				border.bottomLeft = 15;
				border.bottomRight = 15;
			}

			addCodeWidget(codeText[indexCode], border);
			indexCode++;
		}
	}

	if (textList.count() == 0 && codeText.count() != 0)
	{
		Border border;
		border.topLeft = 15;
		border.topRight = 15;
		border.bottomLeft = 15;
		border.bottomRight = 15;
		addCodeWidget(codeText[indexCode], border);
	}

	if (m_message.role != HistoryParser::Role::User)
	{
		setContentsMargins(10, 0, 10, 0);
	}
}

bool MessageWidget::isMaxWidth()
{
	float maxSize = parentWidget()->size().width() * 0.7;

	for (float textWidth : m_textWidth)
	{
		if (m_message.role != HistoryParser::Role::System
				&& (textWidth >= maxSize || m_codeWidgets.count() != 0))
		{
			return true;
		}
	}

	return false;
}

void MessageWidget::setBorder(QWidget *widget, Border border)
{
	widget->setStyleSheet(widget->styleSheet() +
												"border-top-left-radius: " +
												QString::number(border.topLeft) + ";"
												"border-top-right-radius: " +
												QString::number(border.topRight)  + ";"
												"border-bottom-left-radius: " +
												QString::number(border.bottomLeft)  + ";"
												"border-bottom-right-radius: " +
												QString::number(border.bottomRight)  + ";");
}

void MessageWidget::addCodeWidget(QString codeText, Border border)
{
	CodeWidget *code = new CodeWidget(this, codeText, m_menu);

	connect(code, &CodeWidget::changeLanguage,
					this, &MessageWidget::changeLanguage);

	m_codeWidgets.append(code);
	m_layout->addWidget(code);
	setBorder(code, border);
}

void MessageWidget::contextMenuEvent(QContextMenuEvent *event)
{
	m_menu->exec(event->globalPos());
}

void MessageWidget::actionDeleteClicked()
{
	HistoryParser historyParser(this,
															ChatSettings::getSettings(m_chatIndex).fileName);
	historyParser.deleteMessage(m_index);
	deleteLater();
	emit selfDelete();
}

void MessageWidget::changeLanguage(QString language)
{
	CodeWidget *sender = qobject_cast<CodeWidget*>(QObject::sender());
	quint16 i = m_message.content.indexOf("```\n" + sender->getCode() + "\n```");
	quint16 j = i;

	for (;j < m_message.content.length(); j++)
	{
		if (m_message.content[j] == '\n')
		{
			break;
		}
	}

	m_message.content.insert(j, language);
	HistoryParser historyParser(this,
															ChatSettings::getSettings(m_chatIndex).fileName);
	historyParser.editMessage(m_index, m_message.content);
}

void MessageWidget::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	resize();
}

qint8 MessageWidget::getIndex() const
{
	return m_index;
}

qint8 MessageWidget::getChatIndex() const
{
	return m_chatIndex;
}

void MessageWidget::setIndex(qint8 index)
{
	m_index = index;
}

void MessageWidget::selection(QString pattern)
{
	QRegularExpression re(pattern);
	QRegularExpressionMatchIterator matchIterator;
	QTextCharFormat format;
	format.setFontWeight(QFont::Bold);
	quint8 i = 0;

	for (QTextBrowser *textBrowser : qAsConst(m_textBrowsers))
	{
		QTextCursor cursor(textBrowser->document());
		QString text = textBrowser->toPlainText();
		matchIterator = re.globalMatch(text);

		while (matchIterator.hasNext())
		{
			QRegularExpressionMatch match = matchIterator.next();
			cursor.setPosition(match.capturedStart());
			cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
													match.capturedLength());
			cursor.setCharFormat(format);
			m_textWidth[i] += 0.3;
		}

		i++;
	}
}

HistoryParser::Message MessageWidget::getMessage()
{
	return m_message;
}
