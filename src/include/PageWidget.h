#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QWidget>
#include <thread>

namespace Ui
{
  class PageWidget;
}

class PageWidget : public QWidget
{
  Q_OBJECT

public:
  explicit PageWidget(QWidget *parent = nullptr);
  explicit PageWidget(QWidget *parent, const quint16 &allPage);
  explicit PageWidget(QWidget *parent, const quint16 &allPage, const QString &prefix);
  ~PageWidget();

  quint16 getCurrentPage() const;
  quint16 getAllPage() const;
  void setAllPage(const quint16 &allPage);
  void setPrefix(const QString &prefix);
  void setCurrentPage(const quint16 &currentPage);

private:
  Ui::PageWidget *m_ui;
  quint16 m_currentPage;
  quint16 m_allPage;
  QString m_prefix;

  void setPage();

private slots:
  void nextButtonClicked();
  void backButtonClicked();

signals:
  void pageChanged(quint16 &page);
};

#endif // PAGEWIDGET_H
