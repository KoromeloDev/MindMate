#ifndef APIKEYWIDGET_H
#define APIKEYWIDGET_H

#include <QWidget>
#include <QMessageBox>

#include "EditDialog.h"
#include "APIKey.h"

namespace Ui
{
  class APIKeyWidget;
}

class APIKeyWidget : public QWidget
{
  Q_OBJECT

public:
  explicit APIKeyWidget(QWidget *parent = nullptr);
  ~APIKeyWidget();

  QString getKey() const;

private:
  Ui::APIKeyWidget *m_ui;
  QSharedPointer<EditDialog> m_editDialog;
  APIKey m_key;

  bool setTextKey(QString key);

private slots:
  void editKeyClicked();

signals:
  void keyChanged();
};

#endif // APIKEYWIDGET_H
