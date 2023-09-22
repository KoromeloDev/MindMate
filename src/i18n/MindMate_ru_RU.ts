<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru_RU" sourcelanguage="en_US">
<context>
    <name>APIKeyWidget</name>
    <message>
        <source>API key:</source>
        <translation type="obsolete">API ключ:</translation>
    </message>
    <message>
        <source>API key</source>
        <translation type="obsolete">API ключ</translation>
    </message>
    <message>
        <location filename="../ui/APIKeyWidget.ui" line="36"/>
        <location filename="../src/APIKeyWidget.cpp" line="37"/>
        <source>API key: </source>
        <translation>API ключ: </translation>
    </message>
    <message>
        <location filename="../src/APIKeyWidget.cpp" line="45"/>
        <source>Enter your key from the &lt;a href=https://platform.openai.com/account/api-keys&gt;OpenAI site&lt;/a&gt;:</source>
        <translation>Введите свой ключ с &lt;a href=https://platform.openai.com/account/api-keys&gt;сайта OpenAI&lt;/a&gt;:</translation>
    </message>
    <message>
        <location filename="../src/APIKeyWidget.cpp" line="49"/>
        <source>Set OpenAI API key</source>
        <translation>Установить OpenAI API ключ</translation>
    </message>
    <message>
        <location filename="../src/APIKeyWidget.cpp" line="59"/>
        <source>Wrong key</source>
        <translation>Неправильный ключ</translation>
    </message>
    <message>
        <location filename="../src/APIKeyWidget.cpp" line="60"/>
        <source>You entered the wrong key</source>
        <translation>Вы ввели неправильный ключ</translation>
    </message>
</context>
<context>
    <name>ChatGPT</name>
    <message>
        <location filename="../base/src/ChatGPT.cpp" line="77"/>
        <source>Connection failed</source>
        <translation>Сбой подключения</translation>
    </message>
    <message>
        <location filename="../base/src/ChatGPT.cpp" line="125"/>
        <source>The process is busy</source>
        <translation>Поцесс занят</translation>
    </message>
</context>
<context>
    <name>ChatItem</name>
    <message>
        <location filename="../src/ChatItem.cpp" line="133"/>
        <source>Change chat name to</source>
        <translation>Изменить название чата на</translation>
    </message>
</context>
<context>
    <name>ChatSettingsDialog</name>
    <message>
        <location filename="../ui/ChatSettingsDialog.ui" line="17"/>
        <source>Chat Settings</source>
        <translation>Настройки Чата</translation>
    </message>
</context>
<context>
    <name>ChatSettingsWidget</name>
    <message>
        <source>Chat Settings</source>
        <translation type="vanished">Настройки Чата</translation>
    </message>
    <message>
        <location filename="../ui/ChatSettingsWidget.ui" line="246"/>
        <source>Temperature</source>
        <translation>Температура</translation>
    </message>
    <message>
        <location filename="../ui/ChatSettingsWidget.ui" line="271"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Higher values like 0.8 will make the output more random, while lower values like 0.2 will make it more focused and deterministic.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Более высокие значения, такие как 0.8, сделают режим вывода случайным, в то время как более низкое значение, такое как 0.2, сделает его более целенаправленным и детерминированным.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../ui/ChatSettingsWidget.ui" line="307"/>
        <source>Number of responses</source>
        <translation>Количество ответов</translation>
    </message>
    <message>
        <location filename="../ui/ChatSettingsWidget.ui" line="361"/>
        <source>Stop words</source>
        <translation>Стоп-слова</translation>
    </message>
    <message>
        <location filename="../ui/ChatSettingsWidget.ui" line="51"/>
        <source>Presence penalty</source>
        <translation>Штраф за присутствие</translation>
    </message>
    <message>
        <location filename="../ui/ChatSettingsWidget.ui" line="185"/>
        <source>Frequency penalty</source>
        <translation>Штраф за частоту</translation>
    </message>
    <message>
        <location filename="../ui/ChatSettingsWidget.ui" line="332"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;How many chat completion choices to generate for each input message.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Сколько вариантов завершения чата необходимо сгенерировать для каждого входного сообщения.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../ui/ChatSettingsWidget.ui" line="358"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Up to 4 sequences where the API will stop generating further tokens.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;До 4 последовательностей, в которых API перестанет генерировать дальнейшие токены.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../ui/ChatSettingsWidget.ui" line="76"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Positive values penalize new tokens based on whether they appear in the text so far, increasing the model&apos;s likelihood to talk about new topics.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Положительные значения наказывают за новые токены в зависимости от того, появляются ли они в тексте на данный момент, увеличивая вероятность того, что модель заговорит на новые темы.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../ui/ChatSettingsWidget.ui" line="210"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Positive values penalize new tokens based on their existing frequency in the text so far, decreasing the model&apos;s likelihood to repeat the same line verbatim.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Положительные значения наказывают за новые токены на основе их существующей частоты в тексте на данный момент, уменьшая вероятность дословного повторения одной и той же строки моделью.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/ChatSettingsWidget.cpp" line="114"/>
        <source>Add stop word</source>
        <translation>Добавление стоп-слова</translation>
    </message>
</context>
<context>
    <name>CodeWidget</name>
    <message>
        <location filename="../ui/CodeWidget.ui" line="100"/>
        <location filename="../src/CodeWidget.cpp" line="292"/>
        <source>Copy</source>
        <translation>Копировать</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Удалить</translation>
    </message>
    <message>
        <location filename="../src/CodeWidget.cpp" line="274"/>
        <source>Copied</source>
        <translation>Скопировано</translation>
    </message>
</context>
<context>
    <name>EditDialog</name>
    <message>
        <location filename="../ui/EditDialog.ui" line="17"/>
        <source>Editing</source>
        <translation>Редактирование</translation>
    </message>
</context>
<context>
    <name>EditMessageDialog</name>
    <message>
        <location filename="../ui/EditMessageDialog.ui" line="32"/>
        <source>Edit message</source>
        <translation>Редактирование сообщения</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../ui/MainWindow.ui" line="108"/>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location filename="../ui/MainWindow.ui" line="47"/>
        <source>Create chat</source>
        <translation>Создать чат</translation>
    </message>
    <message>
        <location filename="../ui/MainWindow.ui" line="181"/>
        <source>Retry</source>
        <translation>Повторить</translation>
    </message>
    <message>
        <location filename="../ui/MainWindow.ui" line="232"/>
        <source>Stop</source>
        <translation>Остановить</translation>
    </message>
    <message>
        <location filename="../ui/MainWindow.ui" line="382"/>
        <source>User</source>
        <translation>Пользователь</translation>
    </message>
    <message>
        <location filename="../ui/MainWindow.ui" line="387"/>
        <source>System</source>
        <translation>Система</translation>
    </message>
    <message>
        <location filename="../ui/MainWindow.ui" line="392"/>
        <source>Assistant</source>
        <translation>Помощник</translation>
    </message>
    <message>
        <source>Update available</source>
        <translation type="vanished">Доступно обновление</translation>
    </message>
    <message>
        <source>Link to download the application</source>
        <translation type="vanished">Ссылка на скачивание приложения</translation>
    </message>
    <message>
        <location filename="../src/MainWindow.cpp" line="179"/>
        <source> - this is the first sentence in the chat, you should understand what it&apos;s about and name the chat according to its topic. Name it as briefly as possible, but keep the meaning, and try to use signs only where it is really necessary. Also, you should not name the chat like &quot;chat name:&quot;, you should just write the name without unnecessary words.</source>
        <translation> - это первое предложение в чате, вы должны понять, о чем идет речь, и назвать чат в соответствии с его темой. Называйте его как можно короче, но сохраняя смысл, и старайтесь использовать знаки только там, где это действительно необходимо. Также не следует называть чат по типу &quot;название чата:&quot;, нужно просто написать название без лишних слов.</translation>
    </message>
    <message>
        <location filename="../src/MainWindow.cpp" line="248"/>
        <location filename="../src/MainWindow.cpp" line="456"/>
        <source>New chat</source>
        <translation>Новый чат</translation>
    </message>
    <message>
        <location filename="../src/MainWindow.cpp" line="369"/>
        <source>Tokens left</source>
        <translation>Токенов осталось</translation>
    </message>
</context>
<context>
    <name>MessageWidget</name>
    <message>
        <source>Delete</source>
        <translation type="vanished">Удалить</translation>
    </message>
    <message>
        <location filename="../src/MessageWidget.cpp" line="443"/>
        <source>Edit</source>
        <translation>Редактировать</translation>
    </message>
    <message>
        <location filename="../src/MessageWidget.cpp" line="440"/>
        <source>Delete current</source>
        <translation>Удалить текущее</translation>
    </message>
    <message>
        <location filename="../src/MessageWidget.cpp" line="446"/>
        <source>Delete all</source>
        <translation>Удалить всё</translation>
    </message>
    <message>
        <location filename="../src/MessageWidget.cpp" line="437"/>
        <source>Generate</source>
        <translation>Сгенерировать</translation>
    </message>
</context>
<context>
    <name>SettingsWidget</name>
    <message>
        <location filename="../ui/SettingsWidget.ui" line="17"/>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location filename="../ui/SettingsWidget.ui" line="30"/>
        <source>General</source>
        <translation>Общие</translation>
    </message>
    <message>
        <location filename="../ui/SettingsWidget.ui" line="93"/>
        <source>Auto language recognize</source>
        <translation>Автоматическое распознание языка</translation>
    </message>
    <message>
        <location filename="../ui/SettingsWidget.ui" line="51"/>
        <source>Auto naming chat</source>
        <translation>Автоматически называть чаты</translation>
    </message>
    <message>
        <location filename="../ui/SettingsWidget.ui" line="109"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;If the widget with the code does not have a programming language, then it tries to turn to ChatGPT to determine the language (This may take some time).&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Если у виджета с кодом нет языка программирования, то он пытается обратиться к ChatGPT для определения языка (это может занять некоторое время).&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../ui/SettingsWidget.ui" line="67"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Automatically names the chat by sending a request to ChatGPT (This may take some time).&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Автоматически присваивает название чату, отправляя запрос в ChatGPT (это может занять некоторое время).&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../ui/SettingsWidget.ui" line="134"/>
        <source>Chat</source>
        <translation>Чат</translation>
    </message>
    <message>
        <location filename="../ui/SettingsWidget.ui" line="154"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;These settings will initially be applied for each new chat&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Данные настройки будут изначально применяться для каждого нового чата&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../ui/SettingsWidget.ui" line="157"/>
        <source>Default settings for all chats</source>
        <translation>Настройки по умолчанию для всех чатов</translation>
    </message>
    <message>
        <source>API key:</source>
        <translation type="vanished">API ключ:</translation>
    </message>
    <message>
        <source>Check updates</source>
        <translation type="vanished">Проверять обновления</translation>
    </message>
    <message>
        <location filename="../src/SettingsWidget.cpp" line="9"/>
        <source>Version</source>
        <translation>Версия</translation>
    </message>
    <message>
        <source>API key</source>
        <translation type="vanished">API ключ</translation>
    </message>
    <message>
        <source>Enter your key from the &lt;a href=https://platform.openai.com/account/api-keys&gt;OpenAI site&lt;/a&gt;:</source>
        <translation type="vanished">Введите свой ключ с &lt;a href=https://platform.openai.com/account/api-keys&gt;сайта OpenAI&lt;/a&gt;:</translation>
    </message>
    <message>
        <source>Set OpenAI API key</source>
        <translation type="vanished">Установить OpenAI API ключ</translation>
    </message>
    <message>
        <source>Wrong key</source>
        <translation type="vanished">Неверный ключ</translation>
    </message>
    <message>
        <source>You entered the wrong key</source>
        <translation type="vanished">Вы ввели неправильный ключ</translation>
    </message>
</context>
<context>
    <name>SetupDialog</name>
    <message>
        <location filename="../ui/SetupDialog.ui" line="14"/>
        <source>Initial setup</source>
        <translation>Начальная настройка</translation>
    </message>
    <message>
        <location filename="../ui/SetupDialog.ui" line="69"/>
        <source>Done</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/SetupDialog.cpp" line="53"/>
        <location filename="../src/SetupDialog.cpp" line="58"/>
        <location filename="../src/SetupDialog.cpp" line="63"/>
        <source>Wrong key</source>
        <translation>Неправильный ключ</translation>
    </message>
    <message>
        <location filename="../src/SetupDialog.cpp" line="54"/>
        <source>You didn&apos;t provide an API key</source>
        <translation>Вы не предоставили ключ API</translation>
    </message>
    <message>
        <location filename="../src/SetupDialog.cpp" line="59"/>
        <source>Incorrect API key provided</source>
        <translation>Предоставлен неправильный ключ API</translation>
    </message>
</context>
</TS>
