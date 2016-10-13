package botan

import (
	"encoding/json"
	"strconv"
	"github.com/valyala/fasthttp"
	"github.com/rockneurotiko/gorequest"
)

const (
	URL = "https://api.botan.io/track"
	shortenURL = "https://api.botan.io/s/"
)
	
type Answer struct {
	Status string `json:"status"`
	Info   string `json:"info,omitempty"`
}

type sender struct {
	Token string `json:"token"`
	Uid   string `json:"uid"`
	Name  string `json:"name"`
}

func createRequest(getp sender, payload interface{}) *gorequest.SuperAgent {
	return gorequest.New().
		Post(URL).
		DisableKeepAlives(true).
		CloseRequest(true).
		Query(getp).  // Get parameters
		Send(payload) // Post payload
}

type Botan struct {
	Token string
}

func New(token string) Botan {
	return Botan{Token: token}
}

func (self Botan) Track(uid int, message interface{}, name string) (result Answer, err []error) {
	result = Answer{"failed", ""}
	request := createRequest(sender{self.Token, strconv.Itoa(uid), name}, message)

	_, body, err := request.End()

	if err != nil {
		return
	}

	jerr := json.Unmarshal([]byte(body), &result)
	err = []error{jerr}

	return
}

func (self Botan) TrackAsync(uid int, message interface{}, name string, f func(Answer, []error)) {
	go func() {
		ans, err := self.Track(uid, message, name)
		f(ans, err)
	}()
}

func (self Botan) createShortenURL(uid ...int, url string) (string, err) {
	status, shortURL, err := fasthttp.Get(nil, shortenURL+"?token="+self.Token+"&user_ids="+strconv.Itoa(uid)+"&url="+url)
	if err != nil || status != 200 {
		return url, err // Just refund the original URL
	}
	return string(shortURL), nil
}
