#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <thread>

namespace Ui
{
  class SearchWidget;
}

class SearchWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SearchWidget(QWidget *parent = nullptr);
  ~SearchWidget();

  QVector<quint16> search(const QVector<QString> &searchText);
  quint16 getPage() const;
  void setFocus();

private:
  Ui::SearchWidget *m_ui;
  QVector<quint16> m_searchResult;
  QString m_text;
  quint16 m_currentPage;

  void setPage();

private slots:
  void keyClicked(const QString &text);
  void nextButtonClicked();
  void backButtonClicked();

signals:
  void textEnter();
  void pageChanged(quint16 currentPage);
};

#endif // SEARCHWIDGET_H
