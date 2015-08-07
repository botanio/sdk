Botan SDK

Botan is a telegram bot analytics system (based on Yandex.Appmetrika).
Here you can find how to setup Yandex.Appmetrica account and code examples of Botan SDK usage.


## Creating account
 * Register at http://appmetrika.yandex.ru/
 * Create your app there
 * Get API key at settings tab and save it. You will use as token for Botan calls.

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

You can put data to botan using POST or GET methods.
	
For GET method please provide token, uid, name (optional) arguments like that:
	curl -XGET https://api.botan.io/track?token=API_KEY&uid=UID

For POST data please provide json document at post data. Like that:
	curl -XPOST https://api.botan.io/track -d '{"token:" "API_KEY", "uid": "UID"}

Response is a json document like that:
	* on success: {"status": "accepted"}
	* on failure: {"status": "failed"} or {"status": "bad request"}
