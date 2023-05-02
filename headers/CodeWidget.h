#ifndef CODEWIDGET_H
#define CODEWIDGET_H
#pragma once

#include <QWidget>
#include <QClipboard>
#include <QTimer>
#include <QMenu>

#include "qsourcehighliter.h"

namespace Ui
{
class CodeWidget;
}

class CodeWidget : public QWidget
{
	Q_OBJECT

public:
	explicit CodeWidget(QWidget *parent = nullptr, QString code = 0,
											QMenu *menu = nullptr);
	~CodeWidget();

	void resizeWidget(quint16 margin = 0);
	QString getCode();

private:
	Ui::CodeWidget *m_ui;
	QString m_language;
	QString m_code;
	QClipboard *m_clipboard;
	QSourceHighlite::QSourceHighliter *m_highlighter;

	void setCodeAutoHighlighter();

private slots:
	void copyClicked();

signals:
	void contextMenuRequested(QPoint pos);
	void changeLanguage(QString language);

};

#endif // CODEWIDGET_H
