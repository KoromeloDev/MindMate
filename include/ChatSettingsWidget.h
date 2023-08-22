#ifndef CHATSETTINGSWIDGET_H
#define CHATSETTINGSWIDGET_H

#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "ChatSettings.h"
#include "EditDialog.h"

namespace Ui
{
  class ChatSettingsWidget;
}

class ChatSettingsWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ChatSettingsWidget(QWidget *parent = nullptr,
                              ChatSettings settings = {});
  ~ChatSettingsWidget();

  ChatSettings getSettings() const;

protected:
  void changeEvent(QEvent *event);

private:
  Ui::ChatSettingsWidget *m_ui;
  ChatSettings m_settings;
  QSharedPointer<EditDialog> m_editDialog;

  void setParameters();

private slots:
  void modelChanged(const QString &text);
  void temperatureSliderMoved(quint8 position);
  void nSliderMoved(quint8 position);
  void pPSliderMoved(qint8 position);
  void fPSliderMoved(qint8 position);
  void addClicked();
  void addStopWord(const QString text);
  void deleteClicked();

};

#endif // CHATSETTINGSWIDGET_H
