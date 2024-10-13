#include "APIKey.h"

#include "Settings.h"

APIKey::APIKey()
{
  Settings settings;
  QString keyHidden;
  setTextKey(settings.openAIKey, keyHidden);
}

QString APIKey::getKey() const
{
  return m_key;
}

bool APIKey::isNeedKey() const
{
  return m_isNeedKey;
}

bool APIKey::setTextKey(QString key, QString &keyHidden)
{
  if (key.left(8) == "sk-proj-" && key.length() == 164)
  {
    m_key = key;
    keyHidden = m_key.left(3) + "..." +  m_key.right(4);
    m_isNeedKey = false;
  }
  else
  {
    m_isNeedKey = true;
  }

  return !m_isNeedKey;
}
