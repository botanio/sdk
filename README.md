#Botan SDK

Botan is a telegram bot analytics system (based on Yandex.Appmetrica).
Here you can find how to setup Yandex.Appmetrica account and code examples of Botan SDK usage.


## Creating account
 * Register at http://appmetrica.yandex.com/
 * Create your app there
 * Get API key at settings tab and save it. You will use it as token for Botan calls.

## Usage
You can use botan API via libraries for programming language you use.
Or you can use HTTP API.

Currently you can get libraries for following programming languages:
 * JavaScript
 * Python
 * Ruby


### Javacript example
	var botan = require('./botan.js')(token);
    	botan.track(message, 'Start');

### Python example
You need to install requests library to use python botan lib.
You can do it like that:
	pip install requests

Code:

	import botan
	token = 1
	uid = 2    
	messageDict = {}
	print botan.track(token, uid, messageDict, 'Search')

It's required to pass `uid` (user id you get from Telegram) at python lib calls.

### Ruby example
`uid` is a user id you get from Telegram.

	require_relative 'botan'
	token = 1111
	uid = 1
	message = { text: 'text' }
	puts Botan.track(token, uid, message, 'Search')


### HTTP API
Base url is: https://api.botan.io/track

You can put data to botan using POST method.

Url should look like https://api.botan.io/track?token=API_KEY&uid=UID
Please provide json document at post body.

API response is a json document:
	* on success: {"status": "accepted"}
	* on failure: {"status": "failed"} or {"status": "bad request"}
