#include "NewQListWidget.h"
#include "ThemeIcon.h"

#define DOWN_BUTTON_SIZE 48     // Button size
#define DOWN_BUTTON_MIN_SIZE 24 // Button minimum size
#define DOWN_BUTTON_INDENT 16   // Indentation of the button from the edges
#define DOWN_BUTTON_HIDE 0.95   // Value at which the button disappears
#define DOWN_BUTTON_MAX 0.85    /* Value at which the button is not resized
                                   Must be less than DOWN_BUTTON_HIDE */

NewQListWidget::NewQListWidget(QWidget *parent) : QListWidget(parent)
{
  createDownButton();

  connect(verticalScrollBar(), &QScrollBar::valueChanged,
          this, &NewQListWidget::resizeDownButton);
}

void NewQListWidget::createDownButton()
{
  m_downButton = m_downButton.create();

  m_downButton->setIconSize(m_downButton->size() / 8 * 6);
  ThemeIcon::setIcon(*m_downButton, ":/icons/down.svg");
  m_downButton->setFixedSize(DOWN_BUTTON_SIZE, DOWN_BUTTON_SIZE);
  m_downButton->setAutoRaise(true);

  QGridLayout *gridLayout = new QGridLayout;
  gridLayout->addWidget(m_downButton.get(), gridLayout->rowCount(),
                        gridLayout->columnCount(),
                        Qt::AlignBottom | Qt::AlignRight);
  setLayout(gridLayout);
  gridLayout->setContentsMargins(0, 0, DOWN_BUTTON_INDENT + 4,
                                 DOWN_BUTTON_INDENT - 4);

  connect(m_downButton.get(), &QToolButton::clicked,
          this, &NewQListWidget::downButtonClicked);
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
