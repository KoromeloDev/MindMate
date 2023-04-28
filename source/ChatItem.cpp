#include "ChatItem.h"
#include "ui_ChatItem.h"

#include "EditDialog.h"
#include "ThemeIcon.h"

ChatItem::ChatItem(QWidget *parent, QString name, quint8 index)
: QWidget(parent), m_ui(new Ui::ChatItem)
{
	m_ui->setupUi(this);
	m_ui->nameChat->setText(name);
	m_index = index;

	connect(m_ui->editButton, &QToolButton::clicked,
					this, &ChatItem::editClicked);
	connect(m_ui->deleteButton, &QToolButton::clicked,
					this, &ChatItem::deleteCliked);

	ThemeIcon::setIcon(*m_ui->editButton, ":/icons/edit.svg");
	ThemeIcon::setIcon(*m_ui->deleteButton, ":/icons/delete.svg");
}

ChatItem::~ChatItem()
{
	delete m_ui;
}

quint8 ChatItem::getIndex() const
{
	return m_index;
}

void ChatItem::setIndex(quint8 index)
{
	m_index = index;
}

void ChatItem::setNewMessage(bool message)
{
	m_message = message;

	if (message)
	{
		ThemeIcon::setIcon(*m_ui->messageIcon, ":/icons/dot.svg");
	}
	else
	{
		m_ui->messageIcon->setPixmap({});
	}
}

bool ChatItem::thereIsNew() const
{
	return m_message;
}

void ChatItem::editName(QString name)
{
	if (!name.isEmpty())
	{
		QFile chatJson(QDir::currentPath() + "/chats.json");

		if (chatJson.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QByteArray jsonData = chatJson.readAll();
			chatJson.close();

			if (chatJson.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QJsonDocument document = QJsonDocument::fromJson(jsonData);
				QJsonArray chats = document.array();
				QJsonObject object = chats[m_index].toObject();
				object["name"] = name;
				chats[m_index] = object;
				document.setArray(chats);
				chatJson.write(document.toJson());
				chatJson.close();
				m_ui->nameChat->setText(name);
			}
		}
	}
}

void ChatItem::deleteCliked()
{
	QFile chatJson(QDir::currentPath() + "/chats.json");

	if (chatJson.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QByteArray jsonData = chatJson.readAll();
		chatJson.close();

		if (chatJson.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QJsonDocument document = QJsonDocument::fromJson(jsonData);
			QJsonArray chats = document.array();
			QJsonObject object = chats[m_index].toObject();
			QString fileName = object["file_name"].toString();
			fileName = QDir::currentPath() + "/Chat/" + fileName + "_history.json";
			chats.removeAt(m_index);
			document.setArray(chats);
			chatJson.write(document.toJson());
			chatJson.close();

			if (QFile::exists(fileName))
			{
				QFile::remove(fileName);
			}

			deleteLater();
			emit removed(this);
		}
	}
}

void ChatItem::editClicked()
{
	EditDialog *dialog = new EditDialog(this, tr("Change chat name to") + ":",
																			m_ui->nameChat->text());
	connect(dialog, &EditDialog::textChanged, this, &ChatItem::editName);
	connect(dialog, &EditDialog::finished, this, [=]()
	{
		dialog->deleteLater();
	});
	dialog->show();
}
