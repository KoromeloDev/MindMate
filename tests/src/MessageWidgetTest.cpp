#include <QObject>
#include <QTest>

#include "MessageWidget.h"

class MessageWidgetTest : public QObject
{
  Q_OBJECT
  MessageWidget m_messageWidget;

private slots:
  void textDetection();
};

void MessageWidgetTest::textDetection()
{
  QVector<bool> widgetList;
  m_messageWidget.m_message.addMessage("some text");
  widgetList.append(false);
  QCOMPARE(m_messageWidget.m_widgetList, widgetList);

  m_messageWidget.m_message.setMessage("some text \n```\n width code \n```");
  m_messageWidget.newText();
  widgetList.append(true);
  QCOMPARE(m_messageWidget.m_widgetList, widgetList);

  m_messageWidget.m_message.setMessage("```\n just code \n```");
  m_messageWidget.newText();
  widgetList.clear();
  widgetList.append(true);
  QCOMPARE(m_messageWidget.m_widgetList, widgetList);
}

QTEST_MAIN(MessageWidgetTest)
#include "MessageWidgetTest.moc"
