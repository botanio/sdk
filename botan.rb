#usage example:
#
#require_relative 'botan'
#
#puts Botan.track(1111, 1, {'text' => 2}, 'Search')
#
module Botan
  require 'net/http'
  require 'net/https'
  require 'json'

  URI_TEMPLATE = 'http://retention.yurich.serp.yandex.ru/botan?token=%{token}&uid=%{uid}&name=%{name}';

  def self.track(token, uid, message, name = 'Message')
    begin
      uri = URI(URI_TEMPLATE % {token: token, uid: uid, name: name})
      puts uri
      http = Net::HTTP.new(uri.host, uri.port)
      body = JSON.dump(message)

      req =  Net::HTTP::Post.new(uri)
      req.add_field "Content-type", "application/json"
      req.body = body
      # Fetch Request
      res = http.request(req)
      return JSON.parse(res.body)
    rescue StandardError => e
      puts "HTTP Request failed (#{e.message})"
    end
  end
end
