#ifndef CODEWIDGET_H
#define CODEWIDGET_H
#pragma once

#include <QWidget>
#include <QClipboard>
#include <QTimer>
#include <QMenu>

#include "markdownhighlighter.h"

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

private:
	Ui::CodeWidget *m_ui;
	QString m_language;
	QString m_code;
	QClipboard *m_clipboard;
	MarkdownHighlighter *m_highlighter;

private slots:
	void copyClicked();

signals:
	void contextMenuRequested(QPoint pos);

};

#endif // CODEWIDGET_H
