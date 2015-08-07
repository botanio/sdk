<?php

/**
 * Class Botan
 *
 * Usage example:
 *
 * Botan::track(74381, 3, ['text32'], 'wow')
 */
class Botan
{
    /**
     * CURL object
     *
     * @var
     */
    protected static $curl;

    /**
     *
     * Base url
     */
    const BASE_URL = 'https://api.botan.io/track';

    /**
     * @param string $token
     * @param int $uid
     * @param array $message
     * @param string $name
     *
     * @return bool
     */
    public static function track($token, $uid, $message, $name = 'Message')
    {
        if(!function_exists('curl_version')) {
            throw new \Exception('CURL not installed');
        }

        if (!static::$curl) {
            static::$curl = curl_init();
        }

        $options = array(
            CURLOPT_URL => self::BASE_URL,
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_POST => true,
            CURLOPT_POSTFIELDS => [
                'token' => $token,
                'uid' => $uid,
                'name' => $name,
                'message' => json_encode($message)
            ]
        );

        curl_setopt_array(static::$curl, $options);

        $response = json_decode(curl_exec(static::$curl));

        return $response->status === 'accepted' ? true : false;
    }
}
