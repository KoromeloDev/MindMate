#include <QObject>
#include <QTest>

#include "MessageWidget.h"

class MessageWidgetTest : public QObject
{
  Q_OBJECT
  MessageWidget m_messageWidget;

private slots:
  void textDetection_data();
  void textDetection();
};

void MessageWidgetTest::textDetection_data()
{
  QTest::addColumn<QString>("text");
  QTest::addColumn<QVector<bool>>("result");

  QTest::newRow("T") << "text" << QVector<bool>{false};
  QTest::newRow("C") << "```c++\n code \n```" << QVector<bool>{true};
  QTest::newRow("TC") << "text \n```c++\n code \n```" << QVector<bool>{false, true};
  QTest::newRow("CT") << "```c++\n code \n``` text" << QVector<bool>{true, false};
  QTest::newRow("TCT") << "text \n```c++\n code \n```\n text" <<
                          QVector<bool>{false, true, false};
  QTest::newRow("CTC") << "```c++\n code \n```\n text \n```c++\n code \n```" <<
                          QVector<bool>{true, false, true};
}

void MessageWidgetTest::textDetection()
{
  QFETCH(QString, text);
  QFETCH(QVector<bool>, result);

  m_messageWidget.m_message.content.clear();
  m_messageWidget.m_message.addMessage(text);
  m_messageWidget.newText();

  QCOMPARE(m_messageWidget.m_widgetList, result);
}


QTEST_MAIN(MessageWidgetTest)
#include "MessageWidgetTest.moc"
