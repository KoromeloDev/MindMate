#include "ChatItem.h"
#include "ui_ChatItem.h"

#include "ThemeIcon.h"

ChatItem::ChatItem(QWidget *parent, QString name, NewListWidgetItem *item)
: QWidget(parent), m_ui(new Ui::ChatItem)
{
  m_ui->setupUi(this);
  m_name = name;
  m_item = item;
  std::thread t([=]()
  {
    checkWidth();
  });

  connect(m_ui->editButton, &QToolButton::clicked,
          this, &ChatItem::editClicked);
  connect(m_ui->deleteButton, &QToolButton::clicked,
          this, &ChatItem::deleteCliked);
  ThemeIcon::setIcon(*m_ui->editButton, ":/resources/icons/edit.svg");
  ThemeIcon::setIcon(*m_ui->deleteButton, ":/resources/icons/delete.svg");
  t.join();
}

ChatItem::~ChatItem()
{
  delete m_ui;
}

void ChatItem::setNewMessage(const bool &message)
{
  m_message = message;

  if (message)
  {
    ThemeIcon::setIcon(*m_ui->messageIcon, ":/resources/icons/dot.svg");
  }
  else
  {
    m_ui->messageIcon->setPixmap({});
  }
}

bool ChatItem::isNew() const
{
  return m_message;
}

void ChatItem::setName(const QString &name)
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
        const quint8 &index = m_item->getIndex();
        QJsonDocument document = QJsonDocument::fromJson(jsonData);
        QJsonArray chats = document.array();
        QJsonObject object = chats[index].toObject();
        object["name"] = name;
        chats[index] = object;
        document.setArray(chats);
        chatJson.write(document.toJson());
        chatJson.close();
        m_name = name;
        checkWidth();
      }
    }
  }
}

NewListWidgetItem *ChatItem::getItem() const
{
  return m_item;
}

void ChatItem::checkWidth()
{
  QFontMetrics fontSize(m_ui->nameChat->font());
  quint16 nameWidth = fontSize.horizontalAdvance(m_name);
  QString newName = m_name;
  bool changeName = false;

  while (nameWidth > 120)
  {
    newName.chop(1);
    nameWidth = fontSize.horizontalAdvance(newName);
    changeName = true;
  }

  if (changeName)
  {
    newName.append("...");
  }

  m_ui->nameChat->setText(newName);
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
      const quint8 &index = m_item->getIndex();
      QJsonObject object = chats[index].toObject();
      QString fileName = object["file_name"].toString();
      fileName = QDir::currentPath() + "/Chat/" + fileName + "_history.json";
      chats.removeAt(index);
      document.setArray(chats);
      chatJson.write(document.toJson());
      chatJson.close();

      if (QFile::exists(fileName))
      {
        QFile::remove(fileName);
      }

      deleteLater();
      emit removed();
    }
  }
}

void ChatItem::editClicked()
{
  m_editDialog = m_editDialog.create(this, tr("Change chat name to") + ":",
                                     m_name);

  connect(m_editDialog.get(), &EditDialog::textChanged,
          this, &ChatItem::setName);
  connect(m_editDialog.get(), &EditDialog::finished, this, [=](int result)
  {
    if (result == 0)
    {
      m_editDialog.clear();
    }
  });

  m_editDialog->show();
}
