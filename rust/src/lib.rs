extern crate hyper;
extern crate url;
extern crate rustc_serialize;

use std::io::{Read};

use hyper::{Url};
use hyper::client::{Client, IntoUrl};
use hyper::header::ContentType;
use hyper::error::Error as HyperError;
use url::ParseError as UrlError;
use rustc_serialize::{Encodable};
use rustc_serialize::json::{encode, decode};

const URL: &'static str = "https://api.botan.io/track";

#[derive(Debug)]
pub struct Botan {
    url: Url,
    token: String,
}

#[derive(Debug)]
pub enum BotanError {
    Http(HyperError),
    JsonEncoding,
    JsonDecoding(String),
    Failed,
    BadRequest(String),
    Unknown,
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

    pub fn new(token: &str) -> Botan {
        Botan { url: URL.into_url().unwrap(), token: token.to_string() }
    }

    pub fn set_url<U: IntoUrl>(mut self, url: U) -> Result<Botan, UrlError> {
        self.url = try!(url.into_url());
        Ok(self)
    }

    pub fn track<M: Encodable>(&self, uid: u32, message: &M, name: &str) -> Result<(), BotanError> {
        let request = try!(self.prepare_request(uid, message, name));
        let response = try!(Self::make_request(request));
        Self::treat_response(response)
    }

    fn prepare_request<M: Encodable>(&self, uid: u32, message: &M, name: &str) -> Result<BotanRequest, BotanError> {
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
