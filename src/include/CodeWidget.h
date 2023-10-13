#ifndef CODEWIDGET_H
#define CODEWIDGET_H

#include <QClipboard>
#include <QMenu>
#include <QTimer>
#include <QWidget>
#include <qsourcehighliter.h>

#include "ChatGPT.h"

using QSH = QSourceHighlite::QSourceHighliter;
using QSHL = QSH::Language;

namespace Ui
{
  class CodeWidget;
}

class CodeWidget : public QWidget
  {
  Q_OBJECT

public:
  explicit CodeWidget(QWidget *parent = nullptr, QString code = 0,
                      QMenu *menu = nullptr, quint8 index = 0);
  ~CodeWidget();

  void resizeWidget();
  QString getCode() const;
  QSize getSize() const;
  void setEdit(const bool &isEdit);

private:
  Ui::CodeWidget *m_ui;
  QString m_language;
  QString m_code;
  QClipboard *m_clipboard;
  QSharedPointer<QSH> m_highlighter;
  ChatGPT *m_chatGPT;
  QSharedPointer<QTimer> m_timer;
  QSize m_size;
  quint8 m_index;

  void setCodeAutoHighlighter();
  void languageRecognize();
  QSHL getLanguageEnum(QString language);

private slots:
  void copyClicked();
  void timerTimeout();

signals:
  void contextMenuRequested(QPoint pos);
  void changeLanguage(QString language, quint8 index);
};

#endif // CODEWIDGET_H
