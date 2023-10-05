#include "NewQListWidget.h"
#include "ThemeIcon.h"
#include "MessageWidget.h"

#define DOWN_BUTTON_SIZE 48     // Button size
#define DOWN_BUTTON_MIN_SIZE 24 // Button minimum size
#define DOWN_BUTTON_INDENT 16   // Indentation of the button from the edges
#define DOWN_BUTTON_HIDE 0.95   // Value at which the button disappears
#define DOWN_BUTTON_MAX 0.85    /* Value at which the button is not resized
                                   Must be less than DOWN_BUTTON_HIDE */

NewQListWidget::NewQListWidget(QWidget *parent) : QListWidget(parent)
{
  m_searchSelected = 0;
  createDownButton();

  connect(verticalScrollBar(), &QScrollBar::valueChanged,
          this, &NewQListWidget::resizeDownButton);
}

void NewQListWidget::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_F && event->modifiers() == Qt::ControlModifier)
  {
    searchShow();
  }
  else
  {
    QWidget::keyPressEvent(event);
  }
}

void NewQListWidget::createDownButton()
{
  m_downButton = m_downButton.create();
  m_downButton->setIconSize(m_downButton->size() / 8 * 6);
  ThemeIcon::setIcon(*m_downButton, ":/icons/down.svg");
  m_downButton->setFixedSize(DOWN_BUTTON_SIZE, DOWN_BUTTON_SIZE);
  m_downButton->setAutoRaise(true);

  m_layout = m_layout.create(this);
  m_layout->addWidget(m_downButton.get(), 1, 0,
                      Qt::AlignBottom | Qt::AlignRight);
  setLayout(m_layout.get());
  m_layout->setContentsMargins(0, 0, DOWN_BUTTON_INDENT + 4,
                               DOWN_BUTTON_INDENT - 4);

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
            this, &NewQListWidget::searchItems);
    connect(m_searchWidget.get(), &SearchWidget::pageChanged,
            this, &NewQListWidget::searchPageChanged);
  }
  else
  {
    m_searchWidget->setHidden(!m_searchWidget->isHidden());

    if (m_searchWidget->isHidden())
    {
      resetAllFoundColor();
      emit changeFocus();
    }
    else
    {
      searchItems();
      m_searchWidget->setFocus();
    }
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
  if ((qint16)page - 1 == -1)
  {
    return;
  }

  setSelectedFoundColor(m_searchResult[page - 1]);
}

void NewQListWidget::resetAllFoundColor()
{
  for (const quint16 &i : m_searchResult)
  {
    item(i)->setBackground(QBrush());
  }
}

void NewQListWidget::setFoundColor(quint16 index)
{
  item(index)->setBackground(QBrush(QColor(0x6A, 0x9C, 0xEB)));
}

void NewQListWidget::setSelectedFoundColor(quint16 index)
{
  item(index)->setBackground(QBrush(QColor(0xF5, 0xEB, 0x90)));
}

void NewQListWidget::resizeDownButton()
{
  quint32 maxValue = verticalScrollBar()->maximum();
  quint32 currentValue = verticalScrollBar()->value();
  float maxRatio = DOWN_BUTTON_HIDE - DOWN_BUTTON_MAX;

  if (maxValue == 0 || currentValue == 0 || maxRatio <= 0)
  {
    return;
  }

  float ratio = (float)currentValue / maxValue;

  if (ratio > DOWN_BUTTON_HIDE)
  {
    m_downButton->setVisible(false);
    return;
  }

  if (ratio <= DOWN_BUTTON_MAX)
  {
    ratio = 1;
  }
  else
  {
    ratio = 1 - (ratio - DOWN_BUTTON_MAX) / maxRatio;
    float minRatio = (float)DOWN_BUTTON_MIN_SIZE / DOWN_BUTTON_SIZE;
    ratio = minRatio + (1 - minRatio) * ratio;
  }

  m_downButton->setVisible(true);
  m_downButton->setFixedSize(QSize(DOWN_BUTTON_SIZE, DOWN_BUTTON_SIZE) * ratio);
}

void NewQListWidget::downButtonClicked()
{
  scrollToBottom();
}

void NewQListWidget::searchItems()
{
  resetAllFoundColor();
  m_searchResult.clear();
  QVector<QString> messages;

  for (quint16 i = 0; i < count(); ++i)
  {
    MessageWidget *messageWidget = dynamic_cast<MessageWidget*>(
                                   itemWidget(item(i)));
    if (messageWidget != nullptr)
    {
      messages.append(messageWidget->getMessages().getMessage());
    }
  }

  m_searchResult = m_searchWidget->search(messages);
  setAllFoundColor();
}

void NewQListWidget::searchPageChanged(quint16 &page)
{
  if (page == 0)
  {
    return;
  }

  quint16 selected = page - 1;

  if (selected == m_searchSelected && m_searchSelected != 0)
  {
    setFoundColor(m_searchResult[m_searchSelected - 1]);
  }
  else
  {
    setFoundColor(m_searchResult[m_searchSelected]);
  }

  m_searchSelected = selected;
  setSelectedFoundColor(m_searchResult[m_searchSelected]);
  scrollToItem(item(m_searchResult[m_searchSelected]));
}
