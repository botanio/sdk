<?php

/**
 * Wrapper for botan tracker API
 *
 * @author Sonko Dmitry <st.difor@gmail.com>
 */
class BotanTracker
{
	/**
	 * @var Yandex appmetrika application api_key
	 */
	protected $token;

	/**
	 * @var Curl handle
	 */
	protected $ch;

	/**
	 * @var string Tracker url
	 */
	protected $trackerUrl = 'https://api.botan.io/track?';

	public function __construct($token)
	{
		$this->token = $token;
	}

	/**
	 * @param string $uid Uniq ID, for example telegram from_id
	 * @param mixed $message It may be message text for message event or latitude/longitude for location event
	 * @param string $name Event name for show in statistics, eg. Text message, Location, Sticker
	 */
	public function track($uid, $message = null, $name = 'Message')
	{
		if (!$this->ch) {
			$this->ch = curl_init();
			curl_setopt($this->ch, CURLOPT_RETURNTRANSFER, true);
			curl_setopt($this->ch, CURLOPT_POST, true);
			curl_setopt($this->ch, CURLOPT_HTTPHEADER, [
				'Content-type' => 'application/json',
			]);
		}
		$url = $this->trackerUrl . 'token=' . $this->token . '&uid=' . $uid . '&name=' . $name;
		curl_setopt($this->ch, CURLOPT_URL, $url);

		if (!empty($message)) {
			if (!is_array($message)) {
				throw new \Exception('Message must be array');
			}
			curl_setopt($this->ch, CURLOPT_POSTFIELDS, json_encode($message));
		}
		$result = json_decode(curl_exec($this->ch), true);

		return $result;
	}
}