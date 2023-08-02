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
  minSize = minimumSize();
  maxSize = maximumSize();
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

void EditMessageDialog::resize(QSize size)
{
  size += QSize(12, 125);

  if (size.width() > maxSize.width())
  {
    setMaximumWidth(maxSize.width());
    setMinimumWidth(maxSize.width());
    size.setWidth(maxSize.width());
  }
  else if (size.width() < minSize.width())
  {
    setMaximumWidth(minSize.width());
    setMinimumWidth(minSize.width());
    size.setWidth(minSize.width());
  }
  else
  {
    setMaximumWidth(size.width());
    setMinimumWidth(size.width());
  }

  if (size.height() > maxSize.height())
  {
    setMaximumHeight(maxSize.height());
    setMinimumHeight(maxSize.height());
  }
  else if (size.height() < minSize.height())
  {
    setMaximumHeight(minSize.height());
    setMinimumHeight(minSize.height());
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