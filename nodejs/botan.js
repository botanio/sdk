'use strict';

var request = require('request');

var BOTAN_URL = 'https://api.botan.io';
var DEFAULT_NAME = 'Message';

module.exports = function (token) {
    return {
        /**
         * @param {Object} message
         * @param {String} [name='Message']
         * @param {Function} [callback]
         */
        track: function (message, name, callback) {
            if (typeof name === 'function') {
                callback = name;
                name = DEFAULT_NAME;
            }

            request({
                method: 'POST',
                url: BOTAN_URL + '/track',
                qs: {
                    token: token,
                    uid: message.from.id,
                    name: name || DEFAULT_NAME
                },
                json: message
            }, function (error, response, body) {
                if (callback) {
                    callback(error, response, body);
                }
            });
        },
        /**
         * @param {String} uid
         * @param {String} url
         * @param {Function} [callback]
         */
        shortenUrl: function (uid, url, callback) {
            request({
                method: 'GET',
                url: BOTAN_URL + '/s/',
                qs: {
                    token: token,
                    user_ids: uid,
                    url: url
                }
            }, function (error, response, body) {
                if (callback) {
                    callback(error, response, body);
                }
            });
        }
    };
};
