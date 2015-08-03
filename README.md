# Botan SDK
Библиотеки для подключения к аналитике телеграм-ботов Botan

## Создание аккаунта
- зарегистрируйтесь на http://appmetrika.yandex.ru/
- добавьте там приложение
- в настройках приложения посмотрите свой API key и сохраните его. Он понадобится как токен при отправке данных
- скачайте библиотеку на нужном вам языке
- использование конкретных библиотек описано ниже

## Поддерживаемые языки
- JavaScript
- Python
- to be continued... (java, ruby, php)

## Пример использования на JavaScript
    var botan = require('./botan.js')(token);

    botan.track(message, 'Start');

## Пример использования на Python
Требуется библиотека requests. Установка: 
    
    pip install requests

Пример:

    import botan
    token = 1
    uid = 2    
    messageDict = {}
    print botan.track(token, uid, messageDict, 'Search')
В отличие от js, в питоне пока что требуется явно передавать ID юзера в поле uid.

### P.S.
Пулл-реквесты приветствуются!