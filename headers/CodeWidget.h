#ifndef CODEWIDGET_H
#define CODEWIDGET_H
#pragma once

#include <QClipboard>
#include <QMenu>
#include <QTimer>
#include <QWidget>

#include "OpenAIChat.h"
#include "qsourcehighliter.h"

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
  explicit CodeWidget (QWidget *parent = nullptr, QString code = 0,
                       QMenu *menu = nullptr);
  ~CodeWidget ();

  void resizeWidget (quint16 margin = 0);
  QString getCode ();

private:
  Ui::CodeWidget *m_ui;
  QString m_language;
  QString m_code;
  QClipboard *m_clipboard;
  QSH *m_highlighter = nullptr;
  OpenAIChat *m_chatGPT = nullptr;
  QTimer *m_timer = nullptr;

  void setCodeAutoHighlighter ();
  void languageRecognize ();

private slots:
  void copyClicked ();
  void timerTimeout ();

signals:
  void contextMenuRequested (QPoint pos);
  void changeLanguage (QString language);
};

#endif // CODEWIDGET_H
