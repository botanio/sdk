package botan

import (
	"encoding/json"
	"github.com/rockneurotiko/gorequest"
	"github.com/valyala/fasthttp"
	"strconv"
	"strings"
)

const URL = "https://api.botan.io/track"

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

func (self Botan) CreateShortenURL(url string, uid ...int) (string, error) {
	uids := make([]string, len(uid))
	if len(uid) > 1 {
		for k, v := range uid {
			uids[k] = strconv.Itoa(v)
		}
	} else {
		uids[0] = strconv.Itoa(uid[0])
	}
	getRequest := "https://api.botan.io/s/?token=" + self.Token + "&user_ids=" + strings.Join(uids, ",") + "&url=" + url
	status, shortURL, err := fasthttp.Get(nil, getRequest)
	if err != nil || status != 200 {
		return url, err // Just refund the original URL
	}
	return string(shortURL), nil
}
