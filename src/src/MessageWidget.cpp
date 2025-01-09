#include "MessageWidget.h"
#include "ui_MessageWidget.h"

#include "ChatSettings.h"
#include "ThemeIcon.h"
#include "EditMessageDialog.h"
#include "Settings.h"

#define MAX_RATIO 0.7
#define MARGIN 20
#define SPACING 20
#define BORDER 16       // Must be 1 more than the required value

QSharedPointer<EditMessageDialog> m_editDialog;

MessageWidget::MessageWidget(NewListWidgetItem *item,
                             HistoryParser::Messages message, quint8 chatIndex)
: m_ui(new Ui::MessageWidget)
{
  m_ui->setupUi(this);
  m_item = item;
  m_message = message;
  m_chatIndex = chatIndex;
  m_isEdit = false;
  m_ui->pageWidget->setCurrentPage(m_message.selected + 1);

  connect(m_ui->pageWidget, &PageWidget::pageChanged,
          this, &MessageWidget::pageChanged);
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

  m_message = messageWidget->getMessages();
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
  quint16 parentWidth = parentWidget()->size().width();

  if (m_isEdit)
  {
    parentWidth -= 10;
  }

  setFixedWidth(parentWidth);
  m_height = 0;
  m_width = 0;

  if (m_textEdit.length() != 0)
  {
    for (quint8 i = 0; i < m_textEdit.length(); ++i)
    {
      QSize textParameter = getSizeTextEdit(i);
      m_height += textParameter.height();

      if (textParameter.width() > m_width)
      {
        m_width = textParameter.width();
      }
    }
  }
  else
  {
    if (m_message.role == HistoryParser::Role::System)
    {
      m_width = parentWidget()->size().width();
    }
    else
    {
      m_width = parentWidget()->size().width() * MAX_RATIO;
    }
  }

  for (const auto &code : m_codeWidgets)
  {
    qint16 size = code->getSize().height();
    m_height += size;
  }

  if (m_item != nullptr)
  {
    m_item->setSizeHint(QSize(width(), m_height + SPACING +
                              m_ui->pageWidget->minimumHeight()));
  }

  m_ui->pageWidget->setMaximumWidth(m_width);
  setMinimumHeight(m_height);
  --m_queueResize;

  if (m_queueResize == 0)
  {
    emit resizeFinished(getSize());
  }
}

QSize MessageWidget::getSizeTextEdit(quint8 index) const
{
  QTextEdit *textEdit = m_textEdit[index].get();
  quint16 maxSizeWidth = parentWidget()->size().width();
  quint16 sizeWidth = 32;
  quint16 sizeHeight = textEdit->document()->size().toSize().height() + 14;

  if (m_message.role == HistoryParser::Role::System && !m_isEdit)
  {
    textEdit->setAlignment(Qt::AlignCenter);
    sizeWidth = maxSizeWidth;
  }
  else if (m_isEdit)
  {
    sizeWidth = maxSizeWidth;
  }
  else
  {
    maxSizeWidth *= MAX_RATIO;

    if (isMaxWidth(maxSizeWidth))
    {
      sizeWidth = maxSizeWidth;
    }
    else if (m_textWidth.count() != 0)
    {
      sizeWidth += m_textWidth[index];

      if (sizeWidth < m_ui->pageWidget->minimumWidth())
      {
        sizeWidth = m_ui->pageWidget->minimumWidth();
      }
    }
  }

  textEdit->setMaximumHeight(sizeHeight);
  textEdit->setMaximumWidth(sizeWidth);

  return QSize(sizeWidth, sizeHeight);
}

void MessageWidget::createText()
{
  static QRegularExpression re("\\s*(`{3}([^\n]*\n[\\s\\S]*?)\\s*`{3})\\s*");
  QRegularExpressionMatchIterator matchIterator =
                                  re.globalMatch(m_message.getMessage());
  QVector<QString> codeText;
  quint8 indexCode = 0;
  QVector<QString> textList;
  quint8 indexText = 0;
  QVector<qint16> startPosition;
  QVector<qint16> endPosition;
  QString text = m_message.getMessage();

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

    if (isCodeWidget && !m_isEdit)
    {
      addCodeWidget(codeText[indexCode], i + 1 == m_widgetList.count());
      ++indexCode;
    }
    else
    {
      addTextEdit(textList[indexText]);
      ++indexText;
    }
  }

  setContentsMargins(MARGIN, 0, MARGIN, 0);
}

bool MessageWidget::isMaxWidth(quint16 width) const
{
  if (m_codeWidgets.count() != 0)
  {
    return true;
  }

  for (const float &textWidth : m_textWidth)
  {
    if (textWidth >= width)
    {
      return true;
    }
  }

  return false;
}

void MessageWidget::setBorder(QWidget *widget)
{
  widget->setStyleSheet(widget->styleSheet() + QString(
                        "border-bottom-left-radius: %1;"
                        "border-bottom-right-radius: %1;").arg(BORDER-1));
}

void MessageWidget::addCodeWidget(const QString &codeText, bool needSetBorder)
{
  QSharedPointer<CodeWidget> code = code.create(this, codeText, m_menu.get(),
                                                m_currentIndex);

  std::thread t([&]()
  {
    if (needSetBorder)
    {
      setBorder(code.get());
    }
  });

  connect(code.get(), &CodeWidget::changeLanguage,
          this, &MessageWidget::changeLanguage);
  connect(this, &MessageWidget::resizeFinished,
          code.get(), &CodeWidget::resizeWidget);

  code->setEdit(m_isEdit);
  m_codeWidgets.append(code);
  addWidgetToLayout(code.get());

  t.join();
}

void MessageWidget::resizeTimer(quint16 interval)
{
  m_isTimerResize = false;
  m_timer = m_timer.create();
  m_timer->setInterval(interval);
  m_timer->setSingleShot(true);

  connect(m_timer.get(), &QTimer::timeout, this, [=]()
  {
    if (!m_timer.isNull())
    {
      m_timer.clear();
      resize();
      m_isTimerResize = true;
      emit resizeFinished(getSize());
    }
  });

  m_timer->start();
}

void MessageWidget::setPage(bool changeSelected)
{
  if (m_isEdit)
  {
    m_ui->pageWidget->setVisible(false);
    return;
  }

  if (changeSelected)
  {
    ChatSettings сhatSettings;
    HistoryParser historyParser(сhatSettings.getSettings(m_chatIndex).fileName);
    historyParser.setSelected(m_item->getIndex(), m_currentIndex);
    newText(changeSelected);
    emit selfEdit();
  }

  hideDeleteCurrent(m_allMessage == 1);
  m_ui->pageWidget->setAllPage(m_allMessage);
}

void MessageWidget::newText(bool changeSelected)
{
  if (!m_isEdit && changeSelected)
  {
    ChatSettings сhatSettings;
    HistoryParser historyParser(сhatSettings.getSettings(m_chatIndex).fileName);
    m_message = historyParser.getMessages(m_item->getIndex());
  }

  m_widgetList.clear();
  m_textEdit.clear();
  m_textWidth.clear();
  m_codeWidgets.clear();
  createText();

  if (!m_isEdit)
  {
    selection("`([^`]*)`");
    selection("'([^']*)'");
    selection("\"([^\"]*)\"");
    resize();
    resizeTimer();
  }
}

QVector<QColor> MessageWidget::getGradientColors()
{
  QVector<QColor> colors;
  Settings settings;
  settings.readSettings();

  switch (m_message.role)
  {
    case HistoryParser::Role::Assistant:
      colors.append(settings.assistantMessageColor1);
      colors.append(settings.assistantMessageColor2);
      break;

    case HistoryParser::Role::User:
      colors.append(settings.userMessageColor1);
      colors.append(settings.userMessageColor2);
      break;

    case HistoryParser::Role::System:
      colors.append(settings.systemMessageColor1);
      colors.append(settings.systemMessageColor2);
      break;
  }

  return colors;
}

void MessageWidget::init()
{
  m_queueResize = 0;
  m_currentIndex = m_message.selected;
  m_allMessage = m_message.content.size();
  m_generateAction = nullptr;
  m_deleteCurrentAction = nullptr;
  QString color1;
  QString color2;
  QString color3;

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

  if (!m_isEdit)
  {
    m_menu = m_menu.create(this);
    std::thread t([=]()
    {
      m_menu->setStyleSheet("QMenu {"
                              "background-color:" + color2 + ";"
                              "color: " + color1 + ";"
                              "icon-size: 26px;"
                              "font-size: 16px;}"
                            "QMenu::item {"
                              "background-color:" + color2 + ";}"
                            "QMenu::item:selected {"
                              "background-color: " + color3 + ";}");
    });

    m_generateAction = new QAction(ThemeIcon::getIcon(
                                   ":/icons/add.svg"),
                                   tr("Generate"));
    m_deleteCurrentAction = new QAction(ThemeIcon::getIcon(
                                        ":/icons/delete.svg"),
                                        tr("Delete current"));
    m_menu->addAction(m_generateAction);
    m_menu->addAction(ThemeIcon::getIcon(":/icons/edit.svg"),
                      tr("Edit"), this, &MessageWidget::editClicked);
    m_menu->addAction(m_deleteCurrentAction);
    m_menu->addAction(ThemeIcon::getIcon(":/icons/delete.svg"),
                      tr("Delete all"), this, &MessageWidget::deleteAllClicked);
    t.join();
    setContextMenuPolicy(Qt::CustomContextMenu);

    if (m_item != nullptr && m_item->getIndex() == 0)
    {
      hideGenerate();
    }

    connect(m_generateAction, &QAction::triggered,
            this, &MessageWidget::generateClicked);
    connect(m_deleteCurrentAction, &QAction::triggered,
            this, &MessageWidget::deleteCurrentClicked);
    connect(this, &QWidget::customContextMenuRequested, this, [=]()
    {
      if (m_menu != nullptr)
      {
        m_menu->exec(QCursor::pos());
      }
    });
  }

  setPage();
  newText();
}

void MessageWidget::addWidgetToLayout(QWidget *widget)
{
  m_ui->verticalLayout->addWidget(widget);
  m_ui->verticalLayout->setAlignment(getAlignment());
}

void MessageWidget::addTextEdit(QString text)
{
  QSharedPointer<QTextEdit> textEdit = textEdit.create(this);
  textEdit->setFont(QFont(":/fonts/Roboto-Regular.ttf", 11));

  std::thread t([=]()
  {
    QFontMetrics fontSize(textEdit->font());
    m_textWidth.append(fontSize.boundingRect(QRect(), Qt::TextDontClip, text).
                       width());
  });
  textEdit->setStyleSheet("background-color: transparent;"
                          "color: white;"
                          "padding-left:10px;"
                          "padding-right:10px;"
                          "padding-top:5px;"
                          "padding-bottom:5px;"
                          "border: none;");
  textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  textEdit->setText(text);
  textEdit->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(textEdit.get(), &QTextEdit::customContextMenuRequested, this, [=]()
  {
    if (m_menu != nullptr)
    {
      m_menu->exec(QCursor::pos());
    }
  });

  if (m_isEdit)
  {
    textEdit->setAcceptRichText(false);
    connect(textEdit.get(), &QTextEdit::textChanged, this, &MessageWidget::resize);
  }
  else
  {
    textEdit->setReadOnly(true);
  }

  setBorder(textEdit.get());
  m_textEdit.append(textEdit);
  addWidgetToLayout(textEdit.get());
  t.join();
}

void MessageWidget::deleteAllClicked()
{
  ChatSettings сhatSettings;
  HistoryParser historyParser(сhatSettings.getSettings(m_chatIndex).fileName);
  historyParser.deleteMessage(m_item->getIndex());
  deleteLater();
  emit selfDelete();
}

void MessageWidget::deleteCurrentClicked()
{
  ChatSettings сhatSettings;
  HistoryParser historyParser(сhatSettings.getSettings(m_chatIndex).fileName);
  historyParser.deleteMessage(m_item->getIndex(), m_currentIndex);
  newText(true);
  m_currentIndex = m_message.selected;
  m_allMessage = m_message.content.size();
  setPage();
}

void MessageWidget::editClicked()
{
  QWidget *parent = m_item->listWidget()->parentWidget()->parentWidget();
  m_editDialog = m_editDialog.create(parent, this);

  connect(m_editDialog.get(), &EditMessageDialog::rejected, this, [=]()
  {
    m_editDialog.clear();
  });

  connect(m_editDialog.get(), &EditMessageDialog::accepted, this, [=]()
  {
    quint8 index = m_editDialog->getIndex();
    editMessage(m_editDialog->getMessageWidget()->
                getMessages().content[index], index);
    m_editDialog.clear();
  });

  m_editDialog->show();
}

void MessageWidget::generateClicked()
{
  emit generate(m_item->getIndex());
}

void MessageWidget::changeLanguage(QString language, quint8 index)
{
  CodeWidget *sender = qobject_cast<CodeWidget*>(QObject::sender());
  quint16 i = m_message.content[index].indexOf(
              "```\n" + sender->getCode() + "\n```");
  quint16 j = i;
  QString message = m_message.getMessage();

  for (; j < message.length(); ++j)
  {
    if (message[j] == '\n')
    {
      break;
    }
  }

  QString newMessage = m_message.content[index];
  newMessage.insert(j, language);
  editMessage(newMessage);
}

void MessageWidget::pageChanged(quint16 &page)
{
  if (page == 0)
  {
    return;
  }

  m_currentIndex = page - 1;
  setPage(true);
}

void MessageWidget::resizeEvent(QResizeEvent *event)
{
  QWidget::resizeEvent(event);
  resize();
}

void MessageWidget::paintEvent(QPaintEvent *event)
{
  QRect geometry;

  if (m_ui->pageWidget->isHidden())
  {
    geometry.setHeight(size().height());
    geometry.setLeft(MARGIN);
    geometry.setRight(size().width() - MARGIN);
  }
  else
  {
    geometry = m_ui->pageWidget->geometry();
    geometry.setHeight(getSize().height() + geometry.height());
  }

  QPainter painter(this);
  QLinearGradient gradient(geometry.left(), 0, geometry.right(), 0);
  painter.setRenderHint(QPainter::Antialiasing);
  QVector<QColor> colors = getGradientColors();
  gradient.setColorAt(0, colors[0]);
  gradient.setColorAt(1, colors[1]);
  painter.setBrush(gradient);
  painter.setPen(Qt::NoPen);
  painter.drawRoundedRect(geometry, BORDER, BORDER);
  painter.end();
}

quint8 MessageWidget::getCurrentIndex() const
{
  return m_currentIndex;
}

void MessageWidget::hideGenerate()
{
  if (m_generateAction != nullptr)
  {
    m_generateAction->setVisible(false);
  }
}

void MessageWidget::newMessage()
{
  hideDeleteCurrent(false);
  m_currentIndex = m_allMessage;
  ++m_allMessage;
  setPage(true);
}

void MessageWidget::hideDeleteCurrent(bool hide)
{
  if (m_deleteCurrentAction != nullptr)
  {
    m_deleteCurrentAction->setVisible(!hide);
  }
}

Qt::AlignmentFlag MessageWidget::getAlignment()
{
  switch (m_message.role)
  {
    case HistoryParser::Role::Assistant:
      return Qt::AlignLeft;
      break;
    case HistoryParser::Role::System:
      return Qt::AlignHCenter;
      break;
    case HistoryParser::Role::User:
      return Qt::AlignRight;
      break;
    default:
      return {};
  }
}

bool MessageWidget::isTimerResize() const
{
  return m_isTimerResize;
}

NewListWidgetItem *MessageWidget::getItem()
{
  return m_item;
}

qint8 MessageWidget::getChatIndex() const
{
  return m_chatIndex;
}

void MessageWidget::editMessage(QString newContent, quint8 index)
{
  if (m_message.content[index] == newContent)
  {
    return;
  }

  if (newContent.isEmpty())
  {
    if (m_allMessage > 1)
    {
      deleteCurrentClicked();
    }
    else
    {
      deleteAllClicked();
    }

    return;
  }

  ChatSettings сhatSettings;
  HistoryParser historyParser(сhatSettings.getSettings(m_chatIndex).fileName);
  historyParser.editMessage(m_item->getIndex(), index, newContent);
  newText(true);
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

  for (const auto &textEdit : m_textEdit)
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

HistoryParser::Messages MessageWidget::getMessages() const
{
  return m_message;
}

void MessageWidget::updateMessage()
{
  m_message.setMessage(m_textEdit[0]->toPlainText());
}
