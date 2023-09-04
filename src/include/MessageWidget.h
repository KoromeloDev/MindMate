#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QMenu>
#include <QTimer>
#include <thread>

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
                         HistoryParser::Messages message = {},
                         quint8 chatIndex = 0);
  explicit MessageWidget(MessageWidget *messageWidget, bool isEdit);
  ~MessageWidget();

  void resize();
  HistoryParser::Messages getMessages() const;
  void updateMessage();
  qint8 getChatIndex() const;
  NewListWidgetItem *getItem();
  void editMessage(QString newContent, quint8 index = 0);
  QSize getSize() const;
  bool isTimerResize() const;
  quint8 getCurrentIndex() const;
  void hideGenerate();
  void newMessage();

protected:
  void resizeEvent(QResizeEvent *event) override;

private:
  friend class MessageWidgetTest;
  Ui::MessageWidget *m_ui;
  NewListWidgetItem *m_item;
  HistoryParser::Messages m_message;
  QVector<QSharedPointer<CodeWidget>> m_codeWidgets;
  QVector<QSharedPointer<QTextEdit>> m_textEdit;
  QVector<float> m_textWidth;
  quint16 m_width;
  quint16 m_height;
  struct Border
  {
    bool top = false;
    bool bottom = false;
  };
  QSharedPointer<QMenu> m_menu;
  qint8 m_chatIndex;
  bool m_isEdit;
  quint8 m_queueResize;
  QVector<bool> m_widgetList;           //  true == code widget
  QSharedPointer<QTimer> m_timer;
  bool m_isTimerResize;
  quint8 m_currentIndex;
  quint8 m_allMessage;
  QAction *m_generateAction;
  QAction *m_deleteCurrentAction;

  void selection(QString pattern);
  QSize getSizeTextEdit(quint8 index) const;
  void createText();
  bool isMaxWidth(quint16 width) const;
  void setBorder(QWidget *widget, const Border &border);
  inline void addCodeWidget(const QString &codeText, const Border &border);
  void init();
  inline void addWidgetToLayout(QWidget *widget);
  inline void addTextEdit(QString text, Border border);
  void resizeTimer(quint16 interval = 5);
  void setPages(bool changeSelected = false);
  void newText(bool changeSelected = false);
  void hideDeleteCurrent(bool hide);

signals:
  void selfDelete(bool all);
  void selfEdit();
  void resizeFinished(QSize size);
  void generate(quint16 index);

private slots:
  void deleteAllClicked();
  void deleteCurrentClicked();
  void editClicked();
  void generateClicked();
  void changeLanguage(QString language, quint8 index);
  void nextClicked();
  void backClicked();

};

#endif // MESSAGEWIDGET_H
