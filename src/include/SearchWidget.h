#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>

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

  void setPage();

private slots:
  void keyClicked(const QString &text);

signals:
  void textEnter();
  void pageChanged(quint16 &page);
};

#endif // SEARCHWIDGET_H
