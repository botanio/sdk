extern crate rustc_serialize;

extern crate botanio;

use botanio::{Botan};

#[derive(Debug, RustcEncodable)]
struct Message {
    some_metric: u32,
    another_metric: u32,
}

fn main() {
    let token = "1111";
    let uid = 1;
    let name = "Search";
    let message = Message {some_metric: 100, another_metric: 500};

    let botan = Botan::new(token);
    botan.track(uid, name, &message).unwrap();
}
