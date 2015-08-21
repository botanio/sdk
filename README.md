#Botan SDK

[Botan](http://botan.io) is a telegram bot analytics system based on [Yandex.Appmetrica](http://appmetrica.yandex.com/).
In this document you can find how to setup Yandex.Appmetrica account, as well as examples of Botan SDK usage.


## Creating an account
 * Register at http://appmetrica.yandex.com/
 * After registration you will be prompted to create Application. Please use @YourBotName as a name.
 * Save an API key from settings page, you will use it as a token for Botan API calls.
 * Download lib for your language, and use it as described below. Don`t forget to insert your token!

Since we are only getting started, you may discover that some existing reports in AppMetriсa aren't properly working for Telegram bots, like Geography, Gender, Age, Library, Devices, Traffic sources and Network sections. We will polish that later.

## SDK usage
We have libraries for the following languages:
 * JavaScript
 * PHP
 * Python
 * Ruby
 * Rust

More languages (Java, C#, etc.) are coming soon!

Alternatively, you can use Botan API via HTTP calls (please see below).


## JavaScript example
Install npm: `npm install botanio`
```js
var botan = require('botanio')(token);

botan.track(message, 'Start');
```

## PHP example
You need to put the class in a convenient place.
```php
private $token = 'token';

public function _incomingMessage($message_json) {
    $messageObj = json_decode($message_json, true);
    $messageData = $messageObj['message'];
	
    $botan = new Botan($this->token);
    $botan->track($messageData, 'Start');
}
```
Alternatively using Composer
```
$ php composer.phar require iprit/botan-php:*
```
```php
$botan = new IPRIT\BotanSDK\Botan($this->token);
$botan->track($messageData, 'Start');
```



## Python example
You need to install requests library to use python botan lib.
You can do it with

	pip install requests

Code:

	import botan
	token = 1
	uid = 2    
	messageDict = {}
	print botan.track(token, uid, messageDict, 'Search')

It's necessary to pass `uid` (user id you get from Telegram) into python lib calls.

## Ruby example
`uid` is a user id you get from Telegram.

	require_relative 'botan'
	token = 1111
	uid = 1
	message = { text: 'text' }
	puts Botan.track(token, uid, message, 'Search')

## Rust example

```rust

extern crate rustc_serialize;

extern crate botanio;

use botanio::{Botan};

#[derive(Debug, RustcEncodable)]
struct Message {
    some_metric: u32,
    another_metric: u32,
}

fn main() {
    let token = "1111";
    let uid = 1;
    let name = "Search";
    let message = Message {some_metric: 100, another_metric: 500};

    let botan = Botan::new(token);
    botan.track(uid, &message, name).unwrap();
}
```

## HTTP API
The base url is: https://api.botan.io/track

You can put data to Botan using POST method.

The url should look like https://api.botan.io/track?token=API_KEY&uid=UID&name=EVENT_NAME

Please provide a json document as the post body.

API response is a json document:

* on success: {"status": "accepted"}
* on failure: {"status": "failed"} or {"status": "bad request", "info": "some_additional_info_about_error"}
 
##Contribution
We are welcome any contributions as pull-requests!

Feel free to write more libraries for the languages we are not supporting yet.
