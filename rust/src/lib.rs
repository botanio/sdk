extern crate hyper;
extern crate url;
extern crate rustc_serialize;

use std::io::{Read};

use hyper::{Url};
use hyper::client::{Client, IntoUrl};
use hyper::header::ContentType;
use hyper::error::Error as HyperError;
use rustc_serialize::{Encodable};
use rustc_serialize::json::{encode, decode};
use url::ParseError as UrlError;

const URL: &'static str = "https://api.botan.io/track";

#[derive(Debug)]
pub struct Botan {
    url: Url,
    token: String,
}

#[derive(Debug)]
pub enum BotanError {
    Http(HyperError),
    Json,
    Failed,
    BadRequest,
    Unknown,
}

#[derive(Debug, RustcDecodable)]
struct BotanResponse {
    status: String,
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
        let mut url = self.url.clone();
        let s_uid = &uid.to_string();
        let query_pairs: Vec<(&str, &str)> = vec![
            ("token", &self.token),
            ("uid", s_uid),
            ("name", name),
        ];

        url.set_query_from_pairs(query_pairs.into_iter());

        let body = try!(encode(message).map_err(|_| BotanError::Json));
        let client = Client::new();

        let mut response_string = String::new();
        try!(client.post(url)
        .header(ContentType::json())
        .body(&body)
        .send()
        .map_err(BotanError::Http)
        .and_then(|mut resp| resp.read_to_string(&mut response_string).map_err(|err| BotanError::Http(HyperError::Io(err)))));

        match try!(decode::<BotanResponse>(&response_string).map_err(|_| BotanError::Json)).status.as_ref() {
            "accepted"      => Ok(()),
            "failed"        => Err(BotanError::Failed),
            "bad request"   => Err(BotanError::BadRequest),
            _               => Err(BotanError::Unknown),
        }
    }
}

#[test]
fn test_is_default_url_correct() {
    assert!(URL.into_url().is_ok())
}
