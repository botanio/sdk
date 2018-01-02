# ----------------
# dont forget 'pip install requests' first
# ----------------
# usage example:
#
# import botan
#
# print botan.track(1111, 1, {'text':2}, 'Search')

import json

import requests

TRACK_URL = 'https://api.botan.io/track'
SHORTENER_URL = 'https://api.botan.io/s/'


class Botan:
    """
    This class stores the botan_token. It's more practicable to enter the token once, if you
     splitted the bot into many files.
    """
    def __init__(self, token):
        self.token = token

    def track(self, uid, message={}, name='Message'):
        try:
            r = requests.post(
                TRACK_URL,
                params={"token": self.token, "uid": uid, "name": name},
                data=json.dumps(message),
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

    def shorten_url(self, url, user_id=0):
        """
        Shorten URL for specified user of a bot
        """
        try:
            return requests.get(SHORTENER_URL, params={
                'token': self.token,
                'url': url,
                'user_ids': str(user_id),
            }).text
        except:
            return url

    def track_message(self, f):
        """
        Use this method as decorator on functions that handle python-telegram-bot updates.
        """

        def wrapper(bot, update, *args, **kwargs):
            r = f(bot, update, *args, **kwargs)
            text = update.effective_message.text
            uid = update.effective_chat.id
            message = update.effective_message.to_dict()
            self.track(uid, message=message, name=text)
            return r

        return wrapper


def track(token, uid, message, name='Message'):
    try:
        r = requests.post(
            TRACK_URL,
            params={"token": token, "uid": uid, "name": name},
            data=json.dumps(message),
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
