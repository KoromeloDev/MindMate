#include "NewQListWidget.h"
#include "ThemeIcon.h"

#define DOWN_BUTTON_SIZE 48
#define DOWN_BUTTON_INDENT 16
#define DOWN_BUTTON_HIDE 0.95
#define DOWN_BUTTON_MAX 0.8     // Must be less than DOWN_BUTTON_HIDE

NewQListWidget::NewQListWidget(QWidget *parent) : QListWidget(parent)
{
    createDownButton();

    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            this, &NewQListWidget::resizeDownButton);
}

void NewQListWidget::createDownButton()
{
  m_downButton = m_downButton.create();
  m_downButton->setIconSize(QSize(40, 40));
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
    ratio = 1 - ((currentValue - DOWN_BUTTON_MAX * maxValue) /
                (maxRatio * maxValue));
  }

  m_downButton->setVisible(true);
  m_downButton->setFixedSize(QSize(DOWN_BUTTON_SIZE, DOWN_BUTTON_SIZE) * ratio);
}
