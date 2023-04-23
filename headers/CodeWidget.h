#ifndef CODEWIDGET_H
#define CODEWIDGET_H
#pragma once

#include <QWidget>
#include <QClipboard>
#include <QTimer>
#include <QMenu>

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

	void resizeWidget();

private:
	Ui::CodeWidget *m_ui;
	QString m_language;
	QString m_code;
	QClipboard *m_clipboard;

private slots:
	void copyClicked();

signals:
	void contextMenuRequested(QPoint pos);

};

#endif // CODEWIDGET_H
