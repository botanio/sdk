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
 *     $botan = new Botan($this->token);
 *     $botan->track($messageData, 'Start');
 * }
 *
 */
class Botan 
{
	/**
	 * @var string Tracker url
	 */
    protected $template_uri = 'https://api.botan.io/track?token=#TOKEN&uid=#UID&name=#NAME';

	/**
	 * @var string Yandex appmetrika application api_key
	 */
    protected $token;

    function __construct($token) {
        if (empty($token) || !is_string($token)) {
            throw new \Exception('Token should be a string', 2);
        }
        $this->token = $token;
    }

	protected function request($url, $body) {
        $ch = curl_init($url);
        curl_setopt_array($ch, [
            CURLOPT_POST => true,
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_HTTPHEADER => [
                'Content-Type: application/json'
            ],
            CURLOPT_POSTFIELDS => json_encode($body)
        ]);
        $response = curl_exec($ch);
        $error = false;
        if (!$response) {
            $error = true;
        }
        $responseData = json_decode($response, true);
        curl_close($ch);

        return [
            'error' => $error,
            'response' => $responseData,
        ];
    }

    public function track($message, $event_name = 'Message') {
        $uid = $message['from']['id'];
        $url = str_replace(
            ['#TOKEN', '#UID', '#NAME'],
            [$this->token, $uid, $event_name],
            $this->template_uri
        );
        $result = $this->request($url, $message);
        if ($result['error'] || $result['response']['status'] !== 'accepted') {
            throw new \Exception('Error Processing Request', 1);
        }
    }
}