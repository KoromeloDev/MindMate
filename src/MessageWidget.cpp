#include "MessageWidget.h"
#include "ui_MessageWidget.h"

#include "ChatSettings.h"
#include "ThemeIcon.h"
#include "EditMessageDialog.h"

QSharedPointer<EditMessageDialog> m_editDialog;

MessageWidget::MessageWidget(NewListWidgetItem *item,
                             HistoryParser::Message message, quint8 chatIndex) : m_ui(new Ui::MessageWidget)
{
  m_ui->setupUi(this);
  m_item = item;
  m_message = message;
  m_chatIndex = chatIndex;
  m_isEdit = false;
  m_queueResize = 0;
  init();
}

MessageWidget::MessageWidget(MessageWidget *messageWidget, bool isEdit)
: m_ui(new Ui::MessageWidget), m_isEdit(isEdit)
{
  m_ui->setupUi(this);

  if (!isEdit)
  {
    m_item = messageWidget->getItem();
  }
  else
  {
    m_item = nullptr;
  }

  m_message = messageWidget->getMessage();
  m_chatIndex = messageWidget->getChatIndex();
  init();
}

MessageWidget::~MessageWidget()
{
  delete m_ui;
}

void MessageWidget::resize()
{
  if (parentWidget() == nullptr)
  {
    return;
  }

  ++m_queueResize;
  setMaximumWidth(parentWidget()->size().width());
  m_height = 0;
  m_width = 0;

  if (m_textEdit.length() != 0)
  {
    for (quint8 i = 0; i < m_textEdit.length(); ++i)
    {
      QSize textParameter = getSizeTextEdit(m_textEdit[i].get(), i);
      m_height += textParameter.height();

      if (textParameter.width() > m_width)
      {
        m_width = textParameter.width();
      }
    }
  }
  else
  {
    if (m_message.role == HistoryParser::Role::System || m_isEdit)
    {
      m_width = parentWidget()->size().width();
    }
    else
    {
      m_width = parentWidget()->size().width() * 0.7;
    }

    if (m_codeWidgets.count() == 1)
    {
      m_width -= 42;
    }
  }

  for (const auto &code : qAsConst(m_codeWidgets))
  {
    qint16 size = code->getSize().height();
    m_height += size;
  }

  m_ui->horizontalSpacer->changeSize(m_width, 0);
  setMinimumHeight(m_height);

  if (m_item != nullptr)
  {
    m_item->setSizeHint(QSize(width(), m_height+14));
  }

  for (const auto &code : qAsConst(m_codeWidgets))
  {
    code->resizeWidget();
  }

  --m_queueResize;

  if (m_queueResize == 0)
  {
    emit resizeFinished(getSize());
  }
}

QSize MessageWidget::getSizeTextEdit(QTextEdit *textEdit, quint8 index) const
{
  quint16 maxSizeWidth = parentWidget()->size().width();
  quint16 sizeWidth = 32;
  quint16 sizeHeight = textEdit->document()->size().toSize().height() + 14;
  textEdit->setMaximumHeight(sizeHeight);

  if (m_message.role == HistoryParser::Role::System && !m_isEdit)
  {
    textEdit->setAlignment(Qt::AlignCenter);
    sizeWidth = maxSizeWidth - 42;
  }
  else if (m_isEdit)
  {
    sizeWidth = maxSizeWidth - 42;
  }
  else
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

  textEdit->setMaximumWidth(sizeWidth);
  return QSize(sizeWidth, sizeHeight);
}

void MessageWidget::createText()
{
  static QRegularExpression re("\\s*(`{3}([^\n]*\n[\\s\\S]*?)\\s*`{3})\\s*");
  QRegularExpressionMatchIterator matchIterator =
                                  re.globalMatch(m_message.content);
  QStringList codeText;
  quint8 indexCode = 0;
  QStringList textList;
  quint8 indexText = 0;
  QVector<qint16> startPosition;
  QVector<qint16> endPosition;
  QString text = m_message.content;

  if (!m_isEdit)
  {
    while (matchIterator.hasNext())
    {
      QRegularExpressionMatch match = matchIterator.next();
      qint16 start = match.capturedStart();
      qint16 end = match.capturedEnd();
      QString capturedText = match.captured(1);
      startPosition.append(start);
      endPosition.append(end);
      codeText.append(capturedText);
    }
  }

  for (quint8 i = 0; i < startPosition.count(); ++i)
  {
    qint16 end = i == 0 ? 0 : endPosition[i-1];

    if (startPosition[i] != end)
    {
      m_widgetList.append(false);
      qint16 n = startPosition[i] - end;
      QString subText = text.sliced(end, n);
      textList.append(subText);
    }

    m_widgetList.append(true);
  }

  if (startPosition.count() == 0)
  {
    m_widgetList.append(false);
    textList.append(text);
  }
  else
  {
    QString subText = text.mid(endPosition.last(), -1);

    if (!subText.isEmpty())
    {
      m_widgetList.append(false);
      textList.append(subText);
    }
  }

  for (quint8 i = 0; i < m_widgetList.count(); ++i)
  {
    bool isCodeWidget = m_widgetList[i];
    Border border;

    if (i == 0)
    {
      border.topLeft = 15;
      border.topRight = 15;
    }

    if (i + 1 == m_widgetList.count())
    {
      border.bottomLeft = 15;
      border.bottomRight = 15;
    }

    if (isCodeWidget && !m_isEdit)
    {
      addCodeWidget(codeText[indexCode], border);
      ++indexCode;
    }
    else
    {
      addTextEdit(textList[indexText], border);
      ++indexText;
    }
  }

  if (m_message.role != HistoryParser::Role::System)
  {
    setContentsMargins(10, 0, 10, 0);
  }
}

bool MessageWidget::isMaxWidth() const
{
  quint16 maxSize;

  if (!m_isEdit)
  {
    maxSize = parentWidget()->size().width() * 0.7;
  }
  else
  {
    maxSize = 800;
  }

  for (const float &textWidth : m_textWidth)
  {
    if (m_message.role != HistoryParser::Role::System
        && (textWidth >= maxSize || m_codeWidgets.count() != 0))
    {
      return true;
    }
  }

  return false;
}

void MessageWidget::setBorder(QWidget *widget, const Border &border)
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

void MessageWidget::addCodeWidget(const QString &codeText, const Border &border)
{
  QSharedPointer<CodeWidget> code = code.create(this, codeText, m_menu.get());

  connect(code.get(), &CodeWidget::changeLanguage,
          this, &MessageWidget::changeLanguage);

  code->setEdit(m_isEdit);
  m_codeWidgets.append(code);
  addWidgetToLayout(code.get());
  setBorder(code.get(), border);
}

void MessageWidget::resizeTimer()
{
  m_timer = m_timer.create();
  m_timer->setInterval(5);
  m_timer->setSingleShot(true);

  connect(m_timer.get(), &QTimer::timeout, this, [=]()
  {
    m_timer.clear();
    resize();
  });

  m_timer->start();
}

void MessageWidget::init()
{
  QString color1;
  QString color2;
  QString color3;

  switch (m_message.role)
  {
    case HistoryParser::Role::Assistant:
        color1 = "ef745c";
        color2 = "6E3AD5";
        break;

    case HistoryParser::Role::User:
        color1 = "40c9ff";
        color2 = "e81cff";
        break;

    case HistoryParser::Role::System:
        color1 = "e20b8c";
        color2 = "f84b00";
        break;
  }

  setStyleSheet("background: qlineargradient("
                "x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #" + color1 +", "
                "stop: 1 #" + color2 +");");

  if (QPalette().color(QPalette::Window).value() < 128)
  {
    color1 = "white";
    color2 = "black";
    color3 = "#201E1C";
  }
  else
  {
    color1 = "black";
    color2 = "white";
    color3 = "#e6e6e6";
  }

  createText();

  if (!m_isEdit)
  {
    m_menu = m_menu.create(this);
    m_menu->setStyleSheet("QMenu {"
                            "background-color:" + color2 + ";"
                            "color: " + color1 + ";"
                            "icon-size: 26px;"
                            "font-size: 16px;}"
                          "QMenu::item {"
                            "background-color:" + color2 + ";}"
                          "QMenu::item:selected {"
                            "background-color: " + color3 + ";}");
    m_menu->addAction(ThemeIcon::getIcon(":/resources/icons/delete.svg"),
                      tr("Delete"),
                      this, &MessageWidget::actionDeleteClicked);
    m_menu->addAction(ThemeIcon::getIcon(":/resources/icons/edit.svg"),
                      tr("Edit"),
                      this, &MessageWidget::actionEditClicked);
    selection("`([^`]*)`");
    selection("'([^']*)'");
    selection("\"([^\"]*)\"");
    resizeTimer();
  }
}

void MessageWidget::addWidgetToLayout(QWidget *widget)
{
  Qt::AlignmentFlag position;

  switch (m_message.role)
  {
    case HistoryParser::Role::Assistant:
      position = Qt::AlignLeft;
      break;
    case HistoryParser::Role::System:
      position = Qt::AlignHCenter;
      break;
    case HistoryParser::Role::User:
      position = Qt::AlignRight;
      break;
    default:
      return;
  }

  m_ui->verticalLayout->addWidget(widget);
  m_ui->verticalLayout->setAlignment(position);
}

void MessageWidget::addTextEdit(QString text, Border border)
{
  QSharedPointer<QTextEdit> textEdit = textEdit.create(this);
  textEdit->setStyleSheet("color: white;"
                             "padding-left:10px;"
                             "padding-right:10px;"
                             "padding-top:5px;"
                             "padding-bottom:5px;");
  textEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  textEdit->setMinimumSize(40, 40);
  textEdit->setFont(QFont(":/resources/fonts/Roboto-Regular.ttf"));
  m_textEdit.append(textEdit);
  addWidgetToLayout(textEdit.get());
  textEdit->setText(text);
  textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
  QFontMetrics fontSize(textEdit->font());
  m_textWidth.append(fontSize.boundingRect(QRect(), Qt::TextDontClip, text).
                     width());

  connect(textEdit.get(), &QTextEdit::customContextMenuRequested, this, [=]()
  {
    if (m_menu != nullptr)
    {
      m_menu->exec(QCursor::pos());
    }
  });

  if (m_isEdit)
  {
    connect(textEdit.get(), &QTextEdit::textChanged, this, &MessageWidget::resize);
  }
  else
  {
    textEdit->setReadOnly(true);
  }

  setBorder(textEdit.get(), border);
}

void MessageWidget::actionDeleteClicked()
{
  ChatSettings сhatSettings;
  HistoryParser historyParser(this,
                              сhatSettings.getSettings(m_chatIndex).fileName);
  historyParser.deleteMessage(m_item->getIndex());
  deleteLater();
  emit selfDelete();
}

void MessageWidget::actionEditClicked()
{
  QWidget *parent = m_item->listWidget()->parentWidget()->parentWidget();
  m_editDialog = m_editDialog.create(parent, this);

  connect(m_editDialog.get(), &EditMessageDialog::rejected, this, [=]()
  {
    m_editDialog.clear();
  });

  connect(m_editDialog.get(), &EditMessageDialog::accepted, this, [=]()
  {
    editMessage(m_editDialog->getMessageWidget()->getMessage().content);
    m_editDialog.clear();
  });

  m_editDialog->show();
}

void MessageWidget::changeLanguage(QString language)
{
  CodeWidget *sender = qobject_cast<CodeWidget*>(QObject::sender());
  quint16 i = m_message.content.indexOf("```\n" + sender->getCode() + "\n```");
  quint16 j = i;

  for (;j < m_message.content.length(); ++j)
  {
    if (m_message.content[j] == '\n')
    {
      break;
    }
  }

  m_message.content.insert(j, language);
  editMessage(m_message.content);
}

void MessageWidget::resizeEvent(QResizeEvent *event)
{
  QWidget::resizeEvent(event);
  resize();
}

NewListWidgetItem *MessageWidget::getItem()
{
  return m_item;
}

qint8 MessageWidget::getChatIndex() const
{
  return m_chatIndex;
}

void MessageWidget::setItem(NewListWidgetItem *item)
{
  m_item = item;
}

void MessageWidget::editMessage(QString newText)
{
  if (m_message.content == newText)
  {
    return;
  }

  ChatSettings сhatSettings;
  HistoryParser historyParser(this,
                              сhatSettings.getSettings(m_chatIndex).fileName);
  historyParser.editMessage(m_item->getIndex(), newText);
  m_message.content = newText;
  m_widgetList.clear();
  m_textEdit.clear();
  m_codeWidgets.clear();
  createText();
  resize();
  resizeTimer();
  emit selfEdit();
}

QSize MessageWidget::getSize() const
{
  return QSize(m_width, m_height);
}

void MessageWidget::selection(QString pattern)
{
  QRegularExpression re(pattern);
  QRegularExpressionMatchIterator matchIterator;
  QTextCharFormat format;
  format.setFontWeight(QFont::Bold);
  quint8 i = 0;

  for (const auto &textEdit : qAsConst(m_textEdit))
  {
    QTextCursor cursor(textEdit->document());
    QString text = textEdit->toPlainText();
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

    ++i;
  }
}

HistoryParser::Message MessageWidget::getMessage() const
{
  return m_message;
}

void MessageWidget::updateMessage()
{
  QString content;
  content.append(m_textEdit[0]->toPlainText());
  m_message.content = content;
}
