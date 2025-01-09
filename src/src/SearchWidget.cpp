#include "SearchWidget.h"
#include "ui_SearchWidget.h"

SearchWidget::SearchWidget(QWidget *parent)
: QWidget(parent), m_ui(new Ui::SearchWidget)
{
  m_ui->setupUi(this);
  m_ui->pageWidget->setPrefix(QString("%1: ").arg(tr("Found")));

  connect(m_ui->searchEdit, &NewTextEdit::textChanged,
          this, &SearchWidget::keyClicked);
  connect(m_ui->searchEdit, &NewTextEdit::sendText, this, [=]()
  {
    quint16 page = getPage();
    emit pageChanged(page);
  });
  connect(m_ui->pageWidget, &PageWidget::pageChanged,
          this, &SearchWidget::pageChanged);
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
    if (searchText[i].contains(m_text,  Qt::CaseInsensitive))
    {
      m_searchResult.append(i);
    }
  }

  setPage();
  return m_searchResult;
}

quint16 SearchWidget::getPage() const
{
  return m_ui->pageWidget->getCurrentPage();
}

void SearchWidget::setFocus()
{
  m_ui->searchEdit->setFocus();
}

void SearchWidget::setPage()
{
  m_ui->pageWidget->setCurrentPage(1);
  m_ui->pageWidget->setAllPage(m_searchResult.count());
}

void SearchWidget::keyClicked(const QString &text)
{
  m_text = text;
  emit textEnter();
}
