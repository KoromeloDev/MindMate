#include "CodeWidget.h"
#include "ui_CodeWidget.h"

#include "ThemeIcon.h"
#include "Settings.h"
#include "MessageWidget.h"

CodeWidget::CodeWidget(QWidget *parent, QString code, QMenu *menu, quint8 index)
: QWidget(parent), m_ui(new Ui::CodeWidget)
{
  m_ui->setupUi(this);

  ThemeIcon::setIcon(*m_ui->copyButton, ":/icons/copy.svg");

  m_chatGPT = nullptr;
  quint8 i = 0;
  m_code = code;
  m_index = index;

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

    if (symbol == '`' || symbol == '\n' || symbol == ' ' || symbol == '\t')
    {
      ++j;
    }
    else
    {
      break;
    }
  }

  m_code.chop(j);
  m_code.remove(0, i + 1);
  m_ui->codeEdit->setPlainText(m_code);

  m_clipboard = QApplication::clipboard();
  m_ui->languageLabel->setText(m_language);
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

  setCodeAutoHighlighter();
}

CodeWidget::~CodeWidget()
{
  delete m_ui;

  if (m_chatGPT != nullptr)
  {
    delete m_chatGPT;
  }
}

void CodeWidget::resizeWidget()
{
  MessageWidget *messageWidget = dynamic_cast<MessageWidget *>(parentWidget());

  if (messageWidget == nullptr)
  {
    return;
  }

  m_size.setWidth(messageWidget->getSize().width());
  m_size.setHeight(m_ui->codeEdit->document()->size().toSize().height() +
                   m_ui->infoWidget->minimumHeight() + 20);
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

void CodeWidget::setEdit(const bool &isEdit)
{
  m_ui->codeEdit->setReadOnly(!isEdit);
}

QSHL CodeWidget::getLanguageEnum(QString language)
{
  if (language.contains("assembly")) return QSHL::CodeAsm;
  else if (language.contains("bash")) return QSHL::CodeBash;
  else if (language.contains("cmake")) return QSHL::CodeCMake;
  else if (language.contains("css")) return QSHL::CodeCSS;
  else if (language.contains("c#")) return QSHL::CodeCSharp;
  else if (language.contains("cpp") || language.contains("c++"))
    return QSHL::CodeCpp;
  else if (language.contains("c")) return QSHL::CodeC;
  else if (language.contains("go")) return QSHL::CodeGo;
  else if (language.contains("ini")) return QSHL::CodeINI;
  else if (language.contains("json")) return QSHL::CodeJSON;
  else if (language.contains("javascript") || language.contains("js"))
    return QSHL::CodeJs;
  else if (language.contains("java")) return QSHL::CodeJava;
  else if (language.contains("lua")) return QSHL::CodeLua;
  else if (language.contains("php")) return QSHL::CodePHP;
  else if (language.contains("python")) return QSHL::CodePython;
  else if (language.contains("qml")) return QSHL::CodeQML;
  else if (language.contains("rust")) return QSHL::CodeRust;
  else if (language.contains("sql")) return QSHL::CodeSQL;
  else if (language.contains("typescript")) return QSHL::CodeTypeScript;
  else if (language.contains("vex")) return QSHL::CodeVex;
  else if (language.contains("v")) return QSHL::CodeV;
  else if (language.contains("xml")) return QSHL::CodeXML;
  else if (language.contains("yaml")) return QSHL::CodeYAML;

  return QSHL::CodeBash;
}

void CodeWidget::setCodeAutoHighlighter()
{
  QString language = m_language.toLower();
  Settings settings;

  if (language.isEmpty() && settings.languageRecognize)
  {
    m_chatGPT = new ChatGPT(settings.openAIKey);
    ChatSettings chatSettings;
    chatSettings.stop = {" ", ".", "\n"};

    connect(m_chatGPT, &ChatGPT::responseReceived,
            this, &CodeWidget::languageRecognize);

    m_chatGPT->send(m_code +
                    " - is the code, you have to use it to understand "
                    "what kind of programming language it is. "
                    "You must use only the name of the language in your answer "
                    "and nothing else.", chatSettings);
  }
  else if (!language.isEmpty())
  {
    QTextDocument *document = m_ui->codeEdit->document();
    m_highlighter = m_highlighter.create(document);
    m_highlighter->setCurrentLanguage(getLanguageEnum(language));
  }
}

void CodeWidget::languageRecognize()
{
  disconnect(m_chatGPT, &ChatGPT::responseReceived,
             this, &CodeWidget::languageRecognize);

  m_language = m_chatGPT->getAnswerMessage().content[0];
  m_chatGPT->deleteLater();
  m_chatGPT = nullptr;

  if (!m_language.isEmpty())
  {
    while (!m_language.isEmpty())
    {
      if (m_language.last(1) == ' ' || m_language.last(1) == '.' ||
          m_language.last(1) == '\n')
      {
        m_language.remove(m_language.length() - 1, 1);
      }
      else
      {
        break;
      }
    }

    m_language = m_language.toLower();
    emit changeLanguage(m_language, m_index);
  }
}

void CodeWidget::copyClicked()
{
  m_clipboard->setText(m_code);

  if (m_timer == nullptr)
  {
    m_ui->copyButton->setText(tr("Copied") + "!");
    ThemeIcon::setIcon(*m_ui->copyButton, ":/icons/accept.svg");
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
  ThemeIcon::setIcon(*m_ui->copyButton, ":/icons/copy.svg");
}
