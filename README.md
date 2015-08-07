#Botan SDK

[Botan](http://botan.io) is a telegram bot analytics system based on [Yandex.Appmetrica](http://appmetrica.yandex.com/).
In this document you can find how to setup Yandex.Appmetrica account, as well as examples of Botan SDK usage.


## Creating an account
 * Register at http://appmetrica.yandex.com/
 * Create your app there
 * Get an API key at the settings tab and save it. You will use it as a token for Botan API calls.

## SDK usage
We have libraries for the following languages:
 * JavaScript
 * Python
 * Ruby

More languages(PHP, Java, C#, etc.) coming soon!
Or you can call Botan API via the HTTP (described below).


## Javacript example
Install npm: `npm install botanio`
```js
var botan = require('botanio')(token);

botan.track(message, 'Start');
```

## Python example
You need to install requests library to use python botan lib.
You can do it like this:
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


## HTTP API
The base url is: https://api.botan.io/track

You can put data to Botan using POST method.

The url should look like https://api.botan.io/track?token=API_KEY&uid=UID

Please provide a json document as the post body.

API response is a json document:

* on success: {"status": "accepted"}
* on failure: {"status": "failed"} or {"status": "bad request"}
 
##Contribution
We welcome any contributions as pull-requests! Feel free to write more libraries for the languages we are not supporting yet.
