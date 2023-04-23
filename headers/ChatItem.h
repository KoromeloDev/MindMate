#ifndef CHATITEM_H
#define CHATITEM_H
#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui
{
	class ChatItem;
}

class ChatItem : public QWidget
{
	Q_OBJECT

public:
	explicit ChatItem(QWidget *parent = nullptr, QString name = 0,
	                  quint8 index = 0);
	~ChatItem();

	quint8 getIndex() const;
	void setIndex(quint8 index);
	void setNewMessage(bool message);

	bool thereIsNew() const;

private:
	Ui::ChatItem *m_ui;
	quint8 m_index;
	bool m_message;

	void editName(QString name);

private slots:
	void editClicked();
	void deleteCliked();

signals:
	void removed(ChatItem *self);
};
#endif // CHATITEM_H
