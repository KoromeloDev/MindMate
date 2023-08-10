#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QMenu>
#include <QTimer>

#include "HistoryParser.h"
#include "CodeWidget.h"
#include "NewListWidgetItem.h"

namespace Ui
{
  class MessageWidget;
}

class MessageWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MessageWidget(NewListWidgetItem *item = nullptr,
                         HistoryParser::Message message = {},
                         quint8 chatIndex = 0);
  explicit MessageWidget(MessageWidget *messageWidget, bool isEdit);
  ~MessageWidget();

  void resize();
  HistoryParser::Message getMessage() const;
  void updateMessage();
  qint8 getChatIndex() const;
  NewListWidgetItem *getItem();
  void setItem(NewListWidgetItem *item);
  void editMessage(QString newText);
  QSize getSize() const;

protected:
  void resizeEvent(QResizeEvent *event) override;

private:
  Ui::MessageWidget *m_ui;
  NewListWidgetItem *m_item;
  HistoryParser::Message m_message;
  QVector<QSharedPointer<CodeWidget>> m_codeWidgets;
  QVector<QSharedPointer<QTextEdit>> m_textEdit;
  QVector<float> m_textWidth;
  quint16 m_width;
  quint16 m_height;
  struct Border
  {
    quint8 topLeft = 0;
    quint8 topRight = 0;
    quint8 bottomLeft = 0;
    quint8 bottomRight = 0;
  };
  QSharedPointer<QMenu> m_menu;
  qint8 m_chatIndex;
  bool m_isEdit = false;
  quint8 m_queueResize = 0;
  QVector<bool> m_widgetList;           //  true == code widget
  QSharedPointer<QTimer> m_timer;

  void selection(QString pattern);
  QSize getSizeTextEdit(QTextEdit *textEdit, quint8 index) const;
  void createText();
  bool isMaxWidth() const;
  void setBorder(QWidget *widget, const Border &border);
  inline void addCodeWidget(const QString &codeText, const Border &border);
  void init();
  inline void addWidgetToLayout(QWidget *widget);
  inline void addTextEdit(QString text, Border border);
  void resizeTimer();

signals:
  void selfDelete();
  void resizeFinished(QSize size);

private slots:
  void actionDeleteClicked();
  void actionEditClicked();
  void changeLanguage(QString language);

};

#endif // MESSAGEWIDGET_H
