#include "EditDialog.h"
#include "ui_EditDialog.h"

EditDialog::EditDialog(QWidget *parent, QString question)
: QDialog(parent), m_ui(new Ui::EditDialog)
{
	m_ui->setupUi(this);

	connect(m_ui->buttonBox, &QDialogButtonBox::accepted,
					this, &EditDialog::buttonAccepted);
	m_ui->questionText->setText(question);
	m_ui->lineEdit->setText("");
	m_ui->lineEdit->setFocus();
}

EditDialog::EditDialog(QWidget *parent, QString question, QString textEditLine)
: EditDialog(parent, question)
{
	m_ui->lineEdit->setText(textEditLine);
}

EditDialog::~EditDialog()
{
	delete m_ui;
}

void EditDialog::buttonAccepted()
{
	emit textChanged(m_ui->lineEdit->text());
}
