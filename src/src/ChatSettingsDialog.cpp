#include "ChatSettingsDialog.h"
#include "ui_ChatSettingsDialog.h"


ChatSettingsDialog::ChatSettingsDialog(QWidget *parent, quint8 index)
: QDialog(parent), m_ui(new Ui::ChatSettingsDialog)
{
  m_ui->setupUi(this);
  m_index = index;
  ChatSettings сhatSettings;

  m_settingsWidget = m_settingsWidget.create(this,
                                             сhatSettings.getSettings(m_index));

  m_ui->verticalLayout->insertWidget(0, m_settingsWidget.get());
}

ChatSettingsDialog::~ChatSettingsDialog()
{
  delete m_ui;
}

void ChatSettingsDialog::accept()
{
  QFile chatJson(QDir::currentPath() + "/chats.json");

  if (chatJson.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QByteArray jsonData = chatJson.readAll();
    chatJson.close();
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    QJsonArray chats = document.array();

    if (chats.isEmpty())
    {
      return;
    }

    if (chatJson.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      ChatSettings chatSettings =  m_settingsWidget->getSettings();
      QJsonObject object = chats[m_index].toObject();
      object["model"] = chatSettings.model;
      object["temperature"] = chatSettings.temperature;
      object["n"] = chatSettings.n;
      object["stop"] = QJsonArray::fromStringList(chatSettings.stop);
      object["presence_penalty"] = chatSettings.presencePenalty;
      object["frequency_penalty"] = chatSettings.frequencyPenalty;
      chats[m_index] = object;
      document.setArray(chats);
      chatJson.write(document.toJson());
      chatJson.close();
    }
  }

  QDialog::accept();
}
