# ----------------
# dont forget 'pip install aiohttp' first
# also check you using python 3.5
# ----------------
# usage example:
#
# import botan_async
# loop.create_task(track(botan_token, chat_id, msg))
#
# or
#
# async def f():
#   return await track(botan_token, chat_id, msg)

import json
import aiohttp

TRACK_URL = 'https://api.botan.io/track'
SHORTENER_URL = 'https://api.botan.io/s/'

async def track(token, uid, message, name='Message', loop=None):
    params = {"token": token, "uid": uid, "name": name}
    data = json.dumps(message)
    headers = {'Content-type': 'application/json'}
    try:
        with aiohttp.ClientSession(loop=loop) as session:
            async with session.post(TRACK_URL, params=params, data=data, headers=headers) as resp:
                return await resp.json()
    except (aiohttp.errors.ClientError, ValueError) as e:
        print(e)
        return False


async def shorten_url(url, botan_token, user_id, loop=None):
    params = {'token': botan_token, 'url': url, 'user_ids': str(user_id)}
    try:
        with aiohttp.ClientSession(loop=loop) as session:
            async with session.get(SHORTENER_URL, params=params) as resp:
                return await resp.text()
    except aiohttp.errors.ClientError as e:
        print(e)
        return url
