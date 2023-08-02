#ifndef CHATSETTINGSWIDGET_H
#define CHATSETTINGSWIDGET_H

#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "ChatSettings.h"

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

  void setParameters();

private slots:
  void modelChanged(const QString &text);
  void temperatureSliderMoved(quint8 position);
  void nSliderMoved(quint8 position);
  void pPSliderMoved(qint8 position);
  void fPSliderMoved(qint8 position);

};

#endif // CHATSETTINGSWIDGET_H
