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
 * [Go](#go)
 * [Java](#java)
 * [JavaScript](#js)
 * [PHP](#php)
 * [Python](#py)
 * [Ruby](#ruby)
 * [Rust](#rust)

More languages are coming soon.

Alternatively, you can use Botan API via [plain HTTP calls](#http).


## <a name="js"></a>JavaScript example
Install npm: `npm install botanio`
```js
var botan = require('botanio')(token);

botan.track(message, 'Start');
```

## <a name="py"></a>Python example
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

## <a name="php"></a>PHP example
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

## <a name="ruby"></a>Ruby example
`uid` is a user id you get from Telegram.

	require_relative 'botan'
	token = 1111
	uid = 1
	message = { text: 'text' }
	puts Botan.track(token, uid, message, 'Search')

## <a name="rust"></a>Rust example

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
    botan.track(uid, name, &message).unwrap();
}
```

## <a name="java"></a>Java example

```java
try (CloseableHttpAsyncClient client = HttpAsyncClients.createDefault()) {
    client.start();
    Botan botan = new Botan(client, new ObjectMapper());
    botan.track("1111", "1", ImmutableMap.of("some_metric": 100, "another_metric": 500), "Search").get();
}
```

## <a name="go"></a>Go example

```go
package main

import (
	"fmt"

	"github.com/botanio/sdk/go"
)

type Message struct {
	SomeMetric    int
	AnotherMetric int
}

func main() {
	ch := make(chan bool) // Channel for synchronization

	bot := botan.New("1111")
	message := Message{100, 500}

	// Asynchronous track example
	bot.TrackAsync(1, message, "Search", func(ans botan.Answer, err []error) {
		fmt.Printf("Asynchonous: %+v\n", ans)
		ch <- true // Synchronization send
	})

	// Synchronous track example
	ans, _ := bot.Track(1, message, "Search")
	fmt.Printf("Synchronous: %+v\n", ans)

	<-ch // Synchronization receive
}
```

## <a name="http"></a>HTTP API
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
