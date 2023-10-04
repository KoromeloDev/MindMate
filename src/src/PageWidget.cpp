#include "PageWidget.h"
#include "ui_PageWidget.h"

#include "ThemeIcon.h"

PageWidget::PageWidget(QWidget *parent)
: QWidget(parent), m_ui(new Ui::PageWidget)
{
  m_ui->setupUi(this);
  ThemeIcon::setIcon(*m_ui->backButton, ":/icons/back.svg");
  m_currentPage = 0;
  m_allPage = 0;
  m_prefix = 0;
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

  connect(m_ui->nextButton, &QToolButton::clicked,
          this, &PageWidget::nextButtonClicked);
  connect(m_ui->backButton, &QToolButton::clicked,
          this, &PageWidget::backButtonClicked);
}

PageWidget::PageWidget(QWidget *parent, const quint16 &allPage)
: PageWidget(parent)
{
  m_allPage = allPage;
}

PageWidget::PageWidget(QWidget *parent, const quint16 &allPage,
                       const QString &prefix)
: PageWidget(parent, allPage)
{
  m_prefix = prefix;
}

PageWidget::~PageWidget()
{
  delete m_ui;
}

quint16 PageWidget::getCurrentPage() const
{
  return m_currentPage;
}

quint16 PageWidget::getAllPage() const
{
  return m_allPage;
}

void PageWidget::setAllPage(const quint16 &allPage)
{
  m_allPage = allPage;
  setPage();
}

void PageWidget::setPrefix(const QString &prefix)
{
  m_prefix = prefix;
  setPage();
}

void PageWidget::setCurrentPage(const quint16 &currentPage)
{
  m_currentPage = currentPage;
}

void PageWidget::setPage()
{
  if (m_currentPage <= 1 || m_allPage == 0)
  {
    m_ui->backButton->setEnabled(false);
    m_currentPage = 1;
  }
  else
  {
    m_ui->backButton->setEnabled(true);
  }

  if (m_currentPage >= m_allPage)
  {
    m_ui->nextButton->setEnabled(false);
    m_currentPage = m_allPage;
  }
  else
  {
    m_ui->nextButton->setEnabled(true);
  }

  m_ui->label->setText(QString("%1%2/%3").arg(m_prefix,
                            QString::number(m_currentPage),
                            QString::number(m_allPage)));
}

void PageWidget::nextButtonClicked()
{
  ++m_currentPage;
  setPage();
  emit pageChanged(m_currentPage);
}

void PageWidget::backButtonClicked()
{
  --m_currentPage;
  setPage();
  emit pageChanged(m_currentPage);
}
