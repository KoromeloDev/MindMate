#include "CodeWidget.h"
#include "ui_CodeWidget.h"

#include "ThemeIcon.h"
#include "Settings.h"
#include "MessageWidget.h"

CodeWidget::CodeWidget(QWidget *parent, QString code, QMenu *menu)
: QWidget(parent), m_ui(new Ui::CodeWidget)
{
  m_ui->setupUi(this);
  quint8 i = 0;
  m_code = code;

  while (i < code.length())
  {
    if (code[i] == '\n')
    {
      break;
    }

    ++i;
  }

  m_language.append(code.mid(3, i - 3));
  quint8 j = 0;

  while (code.length() != 0)
  {
    QChar symbol = code[code.length() -1 - j];

    if (symbol == '`')
    {
      ++j;
    }
    else
    {
      break;
    }
  }

  m_code.chop(j + 1);
  m_code.remove(0, i + 1);
  m_ui->languageLabel->setText(m_language);
  m_ui->codeEdit->setPlainText(m_code);
  m_clipboard = QApplication::clipboard();
  m_ui->codeEdit->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(m_ui->copyButton, &QToolButton::clicked,
          this, &CodeWidget::copyClicked);
  connect(m_ui->codeEdit, &QTextEdit::customContextMenuRequested,
          this, [=]()
  {
    if (menu != nullptr)
    {
      menu->exec(QCursor::pos());
    }
  });

  ThemeIcon::setIcon(*m_ui->copyButton, ":/resources/icons/copy.svg");
  setCodeAutoHighlighter();
}

CodeWidget::~CodeWidget()
{
  delete m_ui;
}

void CodeWidget::resizeWidget()
{
  MessageWidget *messageWidget = dynamic_cast<MessageWidget *>(parentWidget());

  if (messageWidget == nullptr)
  {
    return;
  }

  m_size.setWidth(messageWidget->getSize().width());
  m_size.setHeight(m_ui->codeEdit->document()->size().toSize().height()+70);
  setMaximumSize(m_size);
}

QString CodeWidget::getCode() const
{
  return m_code;
}

QSize CodeWidget::getSize() const
{
  return m_size;
}

QString CodeWidget::getFullText() const
{
  return "```" + m_ui->languageLabel->text() + "\n" +
         m_ui->codeEdit->toPlainText() + "\n```";
}

void CodeWidget::setEdit(const bool &isEdit)
{
  m_ui->codeEdit->setReadOnly(!isEdit);
}

void CodeWidget::setCodeAutoHighlighter()
{
  QSHL languageEnum;
  QString language = m_language.toLower();
  Settings settings;

  if (language.isEmpty() && settings.languageRecognize)
  {
    m_chatGPT = m_chatGPT.create(this, settings.openAIKey);
    ChatSettings chatSettings;
    chatSettings.stop = {" ", ".", "\n"};

    connect(m_chatGPT.get(), &ChatGPT::responseReceived,
            this, &CodeWidget::languageRecognize);

    m_chatGPT->send(m_code +
                    " - is the code, you have to use it to understand "
                    "what kind of programming language it is. "
                    "You must use only the name of the language in your answer "
                    "and nothing else.", chatSettings);
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

  QTextDocument *document = m_ui->codeEdit->document();
  m_highlighter = m_highlighter.create(document);
  m_highlighter->setCurrentLanguage(languageEnum);
}

void CodeWidget::languageRecognize()
{
  disconnect(m_chatGPT.get(), &ChatGPT::responseReceived,
             this, &CodeWidget::languageRecognize);

  m_language = m_chatGPT->getAnswerMessage().content;
  m_chatGPT->deleteLater();
  m_chatGPT = nullptr;

  if (!m_language.isEmpty())
  {
    while (!m_language.isEmpty())
    {
      if (m_language.last(1) == ' ' || m_language.last(1) == '.'
          || m_language.last(1) == '\n')
      {
        m_language.remove(m_language.length() - 1, 1);
      }
      else
      {
        break;
      }
    }

    m_language = m_language.toLower();
    m_ui->languageLabel->setText(m_language);
    emit changeLanguage(m_language);
    setCodeAutoHighlighter();
  }
}

void CodeWidget::copyClicked()
{
  m_clipboard->setText(m_code);

  if (m_timer == nullptr)
  {
    m_ui->copyButton->setText(tr("Copied") + "!");
    ThemeIcon::setIcon(*m_ui->copyButton, ":/resources/icons/accept.svg");
    m_timer = m_timer.create(this);
    m_timer->setSingleShot(true);
    m_timer->setInterval(1500);

    connect(m_timer.get(), &QTimer::timeout, this, &CodeWidget::timerTimeout);

    m_timer->start();
  }
}

void CodeWidget::timerTimeout()
{
  disconnect(m_timer.get(), &QTimer::timeout, this, &CodeWidget::timerTimeout);

  m_timer->deleteLater();
  m_timer = nullptr;
  m_ui->copyButton->setText(tr("Copy"));
  ThemeIcon::setIcon(*m_ui->copyButton, ":/resources/icons/copy.svg");
}