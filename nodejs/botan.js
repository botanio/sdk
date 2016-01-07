'use strict';

var https = require('https');
var qs = require('querystring');
var DEFAULT_NAME = 'Message';

module.exports = function(token) {
  return {
    /**
     * @param {Object} message
     * @param {String} [name='Message']
     * @param {Function} [cb]
     */
    track: function(message, name, cb) {
      if (typeof name === 'function') {
        cb = name;
        name = DEFAULT_NAME;
      }

      var options = {
        hostname: 'api.botan.io',
        path: '/track?' + qs.stringify({
          token: token,
          uid: message.from.id,
          name: name || DEFAULT_NAME
        }),
        method: 'POST',
        headers: {
          'Content-Type': 'application/json'
        }
      };

      https.request(options, function(res) {
        var data = '';

        res.on('data', function(chunk) {
          data += chunk;
        });

        res.on('end', function() {
          if (cb && typeof cb === 'function') {
            cb(null, res, JSON.parse(data));
          }
        });
      })
      .on('error', function(err) {
        if (cb && typeof cb === 'function') {
          cb(err);
        }
      })
      .end(JSON.stringify(message));
    }
  };
};
