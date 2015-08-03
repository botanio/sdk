var request = require("request");

var url_template = 'http://olmer.haze.yandex.net/botan?token=#token#&uid=#uid#&name=#name#';

module.exports = function(apikey) {
    token = apikey;
    return {
        track: function(message, name) {
            uid = message.from.id;
            name = name ? name : 'Message';
            url = url_template.replace('#token#', token).replace('#uid#', uid).replace('#name#', name);
            //console.log(url);
            request({
                    url: url,
                    method: "POST",
                    json: message
                }, function (error, response, body) {
                    if (!error && response.statusCode === 200) {
                        //console.log(body);
                    }
                    else {
                        console.log("error: " + error);
                        console.log("response.statusCode: " + response.statusCode);
                        console.log("response.statusText: " + response.statusText);
                    }
                }
            );
        }
    };
};
