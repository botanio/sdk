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

URL_TEMPLATE = 'https://api.botan.io/track?token={token}&uid={uid}&name={name}'


def track(token, uid, message, name='Message'):
    global URL_TEMPLATE
    url = URL_TEMPLATE.format(token=str(token), uid=str(uid), name=name)
    headers = {'Content-type': 'application/json'}
    try:
        r = requests.post(url, data=json.dumps(message), headers=headers)
        return r.json()
    except requests.exceptions.Timeout:
        # set up for a retry, or continue in a retry loop
        return False
    except requests.exceptions.RequestException as e:
        # catastrophic error
        print(e)
        return False
