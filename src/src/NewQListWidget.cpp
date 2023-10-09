#include "NewQListWidget.h"
#include "ThemeIcon.h"

#define DOWN_BUTTON_SIZE 48     // Button size
#define DOWN_BUTTON_MIN_SIZE 24 // Button minimum size
#define DOWN_BUTTON_INDENT 16   // Indentation of the button from the edges
#define DOWN_BUTTON_HIDE 400    // Value at which the button disappears
#define DOWN_BUTTON_MAX 800     /* Value at which the button is not resized
                                   Must be more than DOWN_BUTTON_HIDE */

NewQListWidget::NewQListWidget(QWidget *parent) : QListWidget(parent)
{
  m_searchSelected = 0;
  m_shortcut = m_shortcut.create(QKeySequence::Find, this);
  createDownButton();

  connect(verticalScrollBar(), &QScrollBar::rangeChanged,
          this, &NewQListWidget::resizeDownButton);
  connect(verticalScrollBar(), &QScrollBar::valueChanged,
          this, &NewQListWidget::resizeDownButton);
  connect(m_shortcut.get(), &QShortcut::activated,
          this, &NewQListWidget::searchShow);
  connect(qApp, &QApplication::focusChanged,
          this, &NewQListWidget::focusChanged);
}

void NewQListWidget::clear()
{
  QListWidget::clear();
  resetSeachWidget(true);
}

void NewQListWidget::resetSeachWidget(bool hide)
{
  if (m_searchWidget.isNull())
  {
    return;
  }

  if (hide)
  {
    m_searchWidget->setHidden(true);
    m_searchResult.clear();
    return;
  }

  m_searchWidget->setHidden(!m_searchWidget->isHidden());

  if (m_searchWidget->isHidden())
  {
    resetAllFoundColor();
    emit changeFocus();
  }
  else
  {
    textEnter();
    m_searchWidget->setFocus();
  }
}

QVector<QString> NewQListWidget::searchItems()
{
  QVector<QString> text;

  for (quint16 i = 0; i < count(); ++i)
  {
    text.append(item(i)->text());
  }

  return text;
}

void NewQListWidget::itemDelete()
{
  resetSeachWidget(false);
}

void NewQListWidget::itemEdit()
{
  resetSeachWidget(false);
}

void NewQListWidget::createDownButton()
{
  m_downButton = m_downButton.create(this);
  ThemeIcon::setIcon(*m_downButton, ":/icons/down.svg");
  m_downButton->setFixedSize(DOWN_BUTTON_SIZE, DOWN_BUTTON_SIZE);
  m_downButton->setIconSize(m_downButton->size() * 0.875);
  m_downButton->setAutoRaise(true);

  m_layout = m_layout.create(this);
  m_layout->addWidget(m_downButton.get(), 1, 0,
                      Qt::AlignBottom | Qt::AlignRight);
  setLayout(m_layout.get());
  m_layout->setContentsMargins(0, 0, DOWN_BUTTON_INDENT + 4,
                               DOWN_BUTTON_INDENT - 4);
  resizeDownButton();

  connect(m_downButton.get(), &QToolButton::clicked,
          this, &NewQListWidget::downButtonClicked);
}

void NewQListWidget::searchShow()
{
  if (m_layout.isNull())
  {
    return;
  }

  if (m_searchWidget.isNull())
  {
    m_searchWidget = m_searchWidget.create(this);
    m_layout->addWidget(m_searchWidget.get(), 0, 0,
                        Qt::AlignTop | Qt::AlignCenter);
    m_searchWidget->show();

    connect(m_searchWidget.get(), &SearchWidget::textEnter,
            this, &NewQListWidget::textEnter);
    connect(m_searchWidget.get(), &SearchWidget::pageChanged,
            this, &NewQListWidget::searchPageChanged);
  }
  else
  {
    resetSeachWidget(false);
  }
}

void NewQListWidget::setAllFoundColor()
{
  if (m_searchResult.count() == 0)
  {
    return;
  }

  for (const quint16 &i : m_searchResult)
  {
    setFoundColor(i);
  }

  quint16 page = m_searchWidget->getPage();
  if (page == 0)
  {
    return;
  }

  m_searchSelected = page - 1;
  setSelectedFoundColor(m_searchResult[m_searchSelected]);
}

void NewQListWidget::resetAllFoundColor()
{
  for (const quint16 &i : m_searchResult)
  {
    if (item(i) != nullptr)
    {
      item(i)->setBackground(QBrush());
    }
  }
}

void NewQListWidget::setFoundColor(quint16 index)
{
  if (item(index) != nullptr)
  {
    item(index)->setBackground(QBrush(QColor(0x6A, 0x9C, 0xEB)));
  }
}

void NewQListWidget::setSelectedFoundColor(quint16 index)
{
  if (item(index) != nullptr)
  {
    item(index)->setBackground(QBrush(QColor(0xF5, 0xEB, 0x90)));
  }
}

void NewQListWidget::resizeDownButton()
{
  quint32 maxValue = verticalScrollBar()->maximum();
  quint32 currentValue = verticalScrollBar()->value();
  quint32 maxThreshold = maxValue - DOWN_BUTTON_HIDE;

  if (maxValue < DOWN_BUTTON_HIDE || currentValue >= maxThreshold)
  {
    m_downButton->setVisible(false);
    return;
  }

  quint32 minThreshold = maxValue - DOWN_BUTTON_MAX;
  float ratio;

  if (currentValue <= minThreshold)
  {
    ratio = 1;
  }
  else
  {
    float minRatio = (float)DOWN_BUTTON_MIN_SIZE / DOWN_BUTTON_SIZE;
    ratio = 1 - (float)(currentValue - minThreshold) /
            (maxThreshold - minThreshold) * (1 - minRatio);
  }

  m_downButton->setVisible(true);
  m_downButton->setFixedSize(QSize(DOWN_BUTTON_SIZE, DOWN_BUTTON_SIZE) * ratio);
}

void NewQListWidget::downButtonClicked()
{
  scrollToBottom();
}

void NewQListWidget::textEnter()
{
  resetAllFoundColor();
  m_searchResult.clear();
  QVector<QString> text = searchItems();
  m_searchResult = m_searchWidget->search(text);
  setAllFoundColor();
}

void NewQListWidget::searchPageChanged(quint16 &page)
{
  if (page == 0)
  {
    return;
  }

  setFoundColor(m_searchResult[m_searchSelected]);
  m_searchSelected = page - 1;
  setSelectedFoundColor(m_searchResult[m_searchSelected]);
  scrollToItem(item(m_searchResult[m_searchSelected]));
}

void NewQListWidget::focusChanged(QWidget *oldFocus, QWidget *newFocus)
{
  if (newFocus == nullptr || m_searchWidget.isNull() ||
      m_searchWidget->isHidden() || newFocus->parentWidget() == m_searchWidget)
  {
    return;
  }

  m_searchWidget->setHidden(true);
  resetAllFoundColor();
}
