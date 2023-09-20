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

  QTest::newRow("invalid") << "invalid" << 10;

  QTest::newRow("gpt-3.5 - 4k") << "gpt-3.5-turbo" << 4096;
  QTest::newRow("gpt-3.5-new - 4k") << "gpt-3.5-turbo-0613" << 4096;

  QTest::newRow("gpt-3.5 - 16k") << "gpt-3.5-turbo-16k" << 16384;
  QTest::newRow("gpt-3.5-new - 16k") << "gpt-3.5-turbo-16k-0613" << 16384;

  QTest::newRow("gpt-4 - 8k") << "gpt-4" << 8192;
  QTest::newRow("gpt-4-new - 8k") << "gpt-4-0613" << 8192;

  QTest::newRow("gpt-4 - 32k") << "gpt-4-32k" << 32768;
  QTest::newRow("gpt-4-new - 32k") << "gpt-4-32k-0613" << 32768;
}

void ChatSettingsTest::getMaxToken()
{
  QFETCH(QString, model);
  QFETCH(int, result);

  QCOMPARE(chatSettings.getMaxTokens(model), result);
  QCOMPARE(chatSettings.getMaxTokens(), 4096);
}

QTEST_GUILESS_MAIN(ChatSettingsTest)
#include "ChatSettingsTest.moc"
