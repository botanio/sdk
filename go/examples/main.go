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
