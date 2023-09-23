#include <QObject>
#include <QTest>

#include "ChatGPT.h"

class ChatGPTTest : public QObject
{
  Q_OBJECT
  ChatGPT chatGPT;

private slots:
  void getError();
};

void ChatGPTTest::getError()
{
  bool result = false;

  connect(&chatGPT, &ChatGPT::replyError, this, [&result](QString message)
  {
    result = true;
    QCOMPARE(message, "You didn't provide an API key. You need to provide your "
    "API key in an Authorization header using Bearer auth (i.e. Authorization: "
    "Bearer YOUR_KEY), or as the password field (with blank username) if you're"
    " accessing the API from your browser and are prompted for a username and "
    "password. You can obtain an API key from "
    "https://platform.openai.com/account/api-keys.");
  });

  chatGPT.sendJson({});

  result = QTest::qWaitFor([&result]()
  {
    return result;
  }, 5000);

  QVERIFY(result);
  QVERIFY(chatGPT.isError());
}


QTEST_GUILESS_MAIN(ChatGPTTest)
#include "ChatGPTTest.moc"
