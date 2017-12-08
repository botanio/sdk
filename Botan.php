<?php

namespace YourProject;

/**
 * Class Botan
 * @package YourProject
 *
 * Usage:
 *
 * private $token = 'token';
 *
 * public function _incomingMessage($message_json) {
 *     $messageObj = json_decode($message_json, true);
 *     $messageData = $messageObj['message'];
 *
 *     $botan = new YourProject\Botan($this->token);
 *     $botan->track($messageData, 'Start');
 * }
 *
 */

class Botan {

    /**
     * @var string Tracker url
     */
    protected $template_uri = 'https://api.botan.io/track?token=#TOKEN&uid=#UID&name=#NAME';

    /**
     * @var string Url shortener url
     */
    protected $shortener_uri = 'https://api.botan.io/s/?token=#TOKEN&user_ids=#UID&url=#URL';

    /**
     * @var string Yandex AppMetrica application api_key
     */
    protected $token;

    function __construct($token) {
        if (empty($token) || !is_string($token)) {
            throw new \Exception('Token should be a string', 2);
        }
        $this->token = $token;
    }

    public function track($message, $event_name = 'Message') {
        $uid = $message['from']['id'];
        $url = str_replace(
            ['#TOKEN', '#UID', '#NAME'],
            [$this->token, $uid, urlencode($event_name)],
            $this->template_uri
        );
        $result = $this->request($url, $message);
        if ($result['status'] !== 'accepted') {
            throw new \Exception('Error Processing Request', 1);
        }
    }

    public function shortenUrl($url, $user_id) {
        $request_url = str_replace(
            ['#TOKEN', '#UID', '#URL'],
            [$this->token, $user_id, urlencode($url)],
            $this->shortener_uri
        );
        $response = file_get_contents($request_url);
        return $response === false ? $url : $response;
    }

    function getHTTPResponseCode($headers){
        $matches = [];
        $res = preg_match_all('/[\w]+\/\d+\.\d+ (\d+) [\w]+/', $headers[0], $matches);
        if ($res < 1)
        	throw new \Exception('Incorrect response headers');
        $code = intval($matches[1][0]);
        return $code;
    }

    protected function request($url, $body) {
        $options = [
            'http' => [
                'header'  => 'Content-Type: application/json',
                'method'  => 'POST',
                'content' => json_encode($body)
            ]
        ];

        $context = stream_context_create($options);
        $response = file_get_contents($url, false, $context);
        if ($response === false)
            throw new \Exception('Error Processing Request', 1);

        $HTTPCode = $this->getHTTPResponseCode($http_response_header);
        if ($HTTPCode !== 200)
            throw new \Exception("Bad HTTP response code: $HTTPCode".print_r($http_response_header, true));

        $responseData = json_decode($response, true);
        if ($responseData === false)
            throw new \Exception('JSON decode error');

        return $responseData;
    }

}
