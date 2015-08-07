var request = require('request');

var BOTAN_URL = 'https://api.botan.io/track';
var DEFAULT_NAME = 'Message';

module.exports = function (apikey) {
    var token = apikey;
    return {
        /**
         * @param {Object} message
         * @param {String} [name='Message']
         * @param {Function} [callback]
         */
        track: function (message, name, callback) {
            if (typeof name === 'function') {
                name = DEFAULT_NAME;
                callback = name;
            }

            request({
                method: 'POST',
                url: BOTAN_URL,
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
        }
    };
};
