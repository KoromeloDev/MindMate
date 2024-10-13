#include <QObject>
#include <QTest>

#include "ChatSettings.h"

class ChatSettingsTest : public QObject
{
  Q_OBJECT
  ChatSettings chatSettings;

private slots:
  void getMaxToken_data();
  void getMaxToken();
};

void ChatSettingsTest::getMaxToken_data()
{
  QTest::addColumn<QString>("model");
  QTest::addColumn<int>("result");

  QTest::newRow("invalid") << "invalid" << 0;

  QTest::newRow("gpt-4o-mini - 128k") << "gpt-4o-mini" << 128000;
  QTest::newRow("gpt-4o - 128k") << "gpt-4o" << 128000;

  QTest::newRow("gpt-4-turbo - 128k") << "gpt-4-turbo" << 128000;
  QTest::newRow("gpt-4-turbo-preview - 128k") << "gpt-4-turbo-preview" << 128000;


  QTest::newRow("gpt-3.5-turbo - 16k") << "gpt-3.5-turbo" << 16385;

  QTest::newRow("gpt-4 - 8k") << "gpt-4" << 8192;

}

void ChatSettingsTest::getMaxToken()
{
  QFETCH(QString, model);
  QFETCH(int, result);

  QCOMPARE(chatSettings.getMaxTokens(model), result);
  QCOMPARE(chatSettings.getMaxTokens(), 128000);
}

QTEST_GUILESS_MAIN(ChatSettingsTest)
#include "ChatSettingsTest.moc"
