//!
//! Botan is a telegram bot analytics system based on Yandex.Appmetrica.
//!
//! # Creating an account
//! Register at http://appmetrica.yandex.com/.
//!
//! After registration you will be prompted to create Application. Please use @YourBotName as a name.
//!
//! Save an API key from settings page, you will use it as a token for `Botan` initalization.
//!
//! For more information see examples.
//!

extern crate hyper;
extern crate url;
extern crate rustc_serialize;
#[macro_use] extern crate quick_error;

use std::io::{Read};

use hyper::{Url};
use hyper::client::{Client, IntoUrl};
use hyper::header::ContentType;
use hyper::error::Error as HyperError;
use url::ParseError as UrlError;
use rustc_serialize::{Encodable};
use rustc_serialize::json::{encode, decode};

const URL: &'static str = "https://api.botan.io/track";

/// An entry point to the botan.io API
#[derive(Debug)]
pub struct Botan {
    url: Url,
    token: String,
}

quick_error! {
    #[derive(Debug)]
    /// This error could occure during tracking
    pub enum BotanError {
        /// Underlying http error, dns error for example
        Http(err: HyperError) {
            from()
        }
        /// Impossible to encode a message
        JsonEncoding
        /// Impossible to decode response from the server which provided in field
        JsonDecoding(response: String)
        /// Server is unable to process request
        Failed
        /// Malformed request, additional info is provided in field
        BadRequest(info: String)
        /// Invalid status, or "info" field with "accepted" or "failed" status
        Unknown
    }
}

#[derive(Debug, RustcDecodable)]
struct BotanResponse {
    status: String,
    info: Option<String>
}

#[derive(Debug)]
struct BotanRequest {
    url: Url,
    body: String,
}

impl Botan {

    /// Creates a new entry point to Botan API
    pub fn new(token: &str) -> Botan {
        Botan { url: URL.into_url().unwrap(), token: token.to_string() }
    }

    /// Sets custom url to botan.io api server
    pub fn set_url<U: IntoUrl>(mut self, url: U) -> Result<Botan, UrlError> {
        self.url = try!(url.into_url());
        Ok(self)
    }

    /// Send tracking information associated with user
    ///
    /// Parameters:
    ///
    ///   * uid: Supposed to be a telegram user id
    ///   * name: Event name
    ///   * message: Any object that can be encoded to json
    pub fn track<M: Encodable>(&self, uid: i32, name: &str, message: &M) -> Result<(), BotanError> {
        let request = try!(self.prepare_request(uid, name, message));
        let response = try!(Self::make_request(request));
        Self::treat_response(response)
    }

    fn prepare_request<M: Encodable>(&self, uid: i32, name: &str, message: &M) -> Result<BotanRequest, BotanError> {
        let mut url = self.url.clone();
        let s_uid = &uid.to_string();
        let query_pairs: [(&str, &str); 3] = [
            ("token", &self.token),
            ("uid", s_uid),
            ("name", name),
        ];

        url.set_query_from_pairs(query_pairs.into_iter());

        Ok(BotanRequest {
            url: url,
            body: try!(encode(message).map_err(|_| BotanError::JsonEncoding)),
        })
    }

    fn make_request(request: BotanRequest) -> Result<BotanResponse, BotanError> {
        let client = Client::new();

        let mut response_string = String::new();
        try!(
            client.post(request.url)
                  .header(ContentType::json())
                  .body(&request.body)
                  .send()
                  .map_err(BotanError::Http)
                  .and_then(|mut resp| {
                resp.read_to_string(&mut response_string)
                    .map_err(|err| BotanError::Http(HyperError::Io(err)))
            })
        );
        decode(&response_string).map_err(|_| BotanError::JsonDecoding(response_string))

    }

    fn treat_response(response: BotanResponse) -> Result<(), BotanError> {
        match response {
            BotanResponse { ref status, info: None } =>
                match status.as_ref() {
                    "accepted" => Ok(()),
                    "failed"   => Err(BotanError::Failed),
                    _          => Err(BotanError::Unknown),
                },
            BotanResponse { ref status, info: Some(ref info) } =>
                match status.as_ref() {
                    "bad request" => Err(BotanError::BadRequest(info.to_string())),
                    _             => Err(BotanError::Unknown),
                }
        }
    }
}

#[test]
fn test_is_default_url_correct() {
    assert!(URL.into_url().is_ok())
}
