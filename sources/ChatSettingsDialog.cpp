#include "ChatSettingsDialog.h"
#include "ui_ChatSettingsDialog.h"

ChatSettingsDialog::ChatSettingsDialog(QWidget *parent, quint8 index)
: QDialog(parent), m_ui(new Ui::ChatSettingsDialog)
{
  m_ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  m_index = index;
  m_settingsWidget = new ChatSettingsWidget(this,
                                            ChatSettings::getSettings(m_index));

  connect(m_ui->buttonBox, &QDialogButtonBox::accepted,
          this, &QDialog::accept);
  connect(m_ui->buttonBox, &QDialogButtonBox::rejected,
          this, &QDialog::reject);

  m_ui->verticalLayout->insertWidget(0, m_settingsWidget);
}

ChatSettingsDialog::~ChatSettingsDialog()
{
  delete m_ui;
  m_settingsWidget->deleteLater();
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
