#ifndef APIKEY_H
#define APIKEY_H

#include <QObject>

class APIKey
{
public:
  APIKey();

  QString getKey() const;
  bool isNeedKey() const;
  bool setTextKey(QString key, QString &keyHidden);

private:
  QString m_key;
  bool m_isNeedKey;
};

#endif // APIKEY_H
