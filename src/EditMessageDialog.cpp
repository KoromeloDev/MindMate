#include "EditMessageDialog.h"
#include "ui_EditMessageDialog.h"

EditMessageDialog::EditMessageDialog(QWidget *parent,
                                     MessageWidget *messageWidget)
: QDialog(parent), m_ui(new Ui::EditMessageDialog)
{
  if (messageWidget == nullptr)
  {
    return;
  }

  m_ui->setupUi(this);
  QSize size = messageWidget->getSize();
  m_minSize = minimumSize();
  m_maxSize = maximumSize();
  m_index = messageWidget->getCurrentIndex();
  resize(size);
  m_messageWidget = new MessageWidget(messageWidget, true);

  connect(m_messageWidget, &MessageWidget::resizeFinished,
          this, &EditMessageDialog::resize);

  m_ui->scrollArea->setWidget(m_messageWidget);
  m_messageWidget->resize();
}

EditMessageDialog::~EditMessageDialog()
{
  delete m_ui;
}

void EditMessageDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      m_ui->retranslateUi(this);
      break;
    default:
      break;
  }
}

quint8 EditMessageDialog::getIndex() const
{
  return m_index;
}

void EditMessageDialog::resize(QSize size)
{
  size += QSize(12, 70);

  if (size.height() > m_maxSize.height())
  {
    setMaximumHeight(m_maxSize.height());
    setMinimumHeight(m_maxSize.height());
  }
  else if (size.height() < m_minSize.height())
  {
    setMaximumHeight(m_minSize.height());
    setMinimumHeight(m_minSize.height());
  }
  else
  {
    setMaximumHeight(size.height());
    setMinimumHeight(size.height());
  }
}

MessageWidget *EditMessageDialog::getMessageWidget() const
{
  m_messageWidget->updateMessage();
  return m_messageWidget;
}
