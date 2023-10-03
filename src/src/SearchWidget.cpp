#include "SearchWidget.h"
#include "ui_SearchWidget.h"
#include "ThemeIcon.h"

SearchWidget::SearchWidget(QWidget *parent)
: QWidget(parent), m_ui(new Ui::SearchWidget)
{
  m_ui->setupUi(this);
  ThemeIcon::setIcon(*m_ui->backButton, ":/icons/back.svg");
  m_currentPage = 0;
  setPage();

  std::thread t([=]()
  {
    QIcon backIcon = ThemeIcon::getIcon(":/icons/back.svg");
    m_ui->backButton->setIcon(backIcon);
    QPixmap pixmap = backIcon.pixmap(backIcon.actualSize(QSize(32, 32)));
    QTransform transform;
    transform.scale(-1, 1);
    pixmap = pixmap.transformed(transform);
    m_ui->nextButton->setIcon(QIcon(pixmap));
  });
  t.detach();

  connect(m_ui->searchEdit, &NewTextEdit::keyClicked,
          this, &SearchWidget::keyClicked);
  connect(m_ui->nextButton, &QToolButton::clicked,
          this, &SearchWidget::nextButtonClicked);
  connect(m_ui->backButton, &QToolButton::clicked,
          this, &SearchWidget::backButtonClicked);
}

SearchWidget::~SearchWidget()
{
  delete m_ui;
}

QVector<quint16> SearchWidget::search(const QVector<QString> &searchText)
{
  m_searchResult.clear();

  if (m_text.isEmpty() || searchText.count() == 0)
  {
    setPage();
    return {};
  }

  for (quint16 i = 0; i < searchText.count(); ++i)
  {
    if (searchText[i].contains(m_text))
    {
      m_searchResult.append(i);
    }
  }

  setPage();
  return m_searchResult;
}

quint16 SearchWidget::getPage() const
{
  return m_currentPage;
}

void SearchWidget::setFocus()
{
  m_ui->searchEdit->setFocus();
}

void SearchWidget::setPage()
{
  quint16 allResult = m_searchResult.count();

  if (m_currentPage <= 1)
  {
    m_ui->backButton->setEnabled(false);
    m_currentPage = 1;
  }
  else
  {
    m_ui->backButton->setEnabled(true);
  }

  if (m_currentPage >= allResult)
  {
    m_ui->nextButton->setEnabled(false);
    m_currentPage = allResult;
  }
  else
  {
    m_ui->nextButton->setEnabled(true);
  }

  if (allResult == 0)
  {
    m_ui->backButton->setEnabled(false);
  }
  else
  {
    m_ui->backButton->setEnabled(true);
  }

  m_ui->foundLabel->setText(QString("%1: %2/%3").arg(tr("Found"),
                            QString::number(m_currentPage),
                            QString::number(allResult)));
}

void SearchWidget::keyClicked(const QString &text)
{
  m_text = text;
  emit textEnter();
}

void SearchWidget::nextButtonClicked()
{
  ++m_currentPage;
  setPage();
  emit pageChanged(m_currentPage);
}

void SearchWidget::backButtonClicked()
{
  --m_currentPage;
  setPage();
  emit pageChanged(m_currentPage);
}
