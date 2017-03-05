# ----------------
# dont forget 'pip install requests' first
# ----------------
# usage example:
#
# import botan
#
# print botan.track(1111, 1, {'text':2}, 'Search')

import requests
import json


TRACK_URL = 'https://api.botan.io/track'
SHORTENER_URL = 'https://api.botan.io/s/'


def make_json(message, inline=False):
    """
        JSON serialize for track() method
        :param is_inline:
        :param message:
        :type message: telegram message object
        :type is_inline: bool
        :return: json
    """
    data = {}
    data['from'] = {}
    data['from']['id'] = message.from_user.id
    if message.from_user.username is not None:
            data['from']['username'] = message.from_user.username
    if inline is False:
        data['message_id'] = message.message_id
        data['chat'] = {}
        data['chat']['id'] = message.chat.id
    else:
        data['message_id'] = message.id
    return data


def track(token, uid, message, name='Message', inline=False):
    try:
        r = requests.post(
            TRACK_URL,
            params={"token": token, "uid": uid, "name": name},
            data=json.dumps(make_json(message, inline)),
            headers={'Content-type': 'application/json'},
        )
        return r.json()
    except requests.exceptions.Timeout:
        # set up for a retry, or continue in a retry loop
        return False
    except (requests.exceptions.RequestException, ValueError) as e:
        # catastrophic error
        print(e)
        return False


def shorten_url(url, botan_token, user_id):
    """
    Shorten URL for specified user of a bot
    """
    try:
        return requests.get(SHORTENER_URL, params={
            'token': botan_token,
            'url': url,
            'user_ids': str(user_id),
        }).text
    except:
        return url
