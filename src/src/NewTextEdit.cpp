#include "NewTextEdit.h"
#include "ui_NewTextEdit.h"

NewTextEdit::NewTextEdit(QWidget *parent)
: QTextEdit(parent), m_ui(new Ui::NewTextEdit)
{
  m_ui->setupUi(this);
  m_text = "";
  setAcceptRichText(false);
  setFocus();
  verticalScrollBar()->setSingleStep(5);

  connect(this, &QTextEdit::textChanged, this, &NewTextEdit::resizeTextInput);
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

void NewTextEdit::clippingStart(QString &text)
{
  if (text.isEmpty())
  {
    return;
  }

  quint16 i = 0;

  for (const QChar &symbol : text)
  {
    if (isIndent(symbol))
    {
      ++i;
    }
    else
    {
      break;
    }
  }

  text = text.last(text.length() - i);
}

void NewTextEdit::clippingEnd(QString &text)
{
  if (text.isEmpty())
  {
    return;
  }

  quint16 i = 0;

  while(true)
  {
    quint16 index = text.length() -1 - i;
    QChar symbol = text[index];

    if (index >= text.length() || !isIndent(symbol))
    {
      break;
    }

    ++i;
  }

  text = text.first(text.length() - i);
}

bool NewTextEdit::isIndent(QChar symbol)
{
  return symbol == '\n' || symbol.isSpace() || symbol == '\t' || symbol.isNull();
}

void NewTextEdit::keyPressEvent(QKeyEvent *event)
{
  if (event->modifiers() == Qt::ShiftModifier ||
     (event->key() != Qt::Key_Return && event->key() != Qt::Key_Enter))
  {
    QTextEdit::keyPressEvent(event);
    QString text = toPlainText();

    if (m_text != text)
    {
      m_text = text;
      emit textChanged(text);
    }

    return;
  }

  QString text = toPlainText();
  clippingStart(text);
  clippingEnd(text);
  emit sendText(text);
}

void NewTextEdit::resizeEvent(QResizeEvent *event)
{
  QTextEdit::resizeEvent(event);
  resizeTextInput();
}
