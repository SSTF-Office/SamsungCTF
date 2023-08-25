mod errors;
mod jwt;
mod user;
mod mybox;

use std::convert::Infallible;
use std::net::IpAddr;

use hyper::server::conn::AddrStream;
use hyper::service::{make_service_fn, service_fn};
use hyper::{Body, Method, Request, Response, Server, StatusCode};

use rust_embed::RustEmbed;


#[derive(RustEmbed)]
#[folder = "$CARGO_MANIFEST_DIR/resources"]
pub struct Asset;


fn set_path(path: &str) -> String {
    let path = path.replace("/", "");
    if path.trim().is_empty() {
        "index.html".to_string()
    } else {
        path
    }
}

fn handle_embedded_file(path: &str) -> Body {
    let path = set_path(path);
    match Asset::get(&path) {
        Some(content) => Body::from(content.data.into_owned()),
        None => Body::from("404 Not Found"),
    }
}

async fn http_handler(client_ip: IpAddr, req: Request<Body>) -> Result<Response<Body>, Infallible> {
    println!("[{}] [{}] [{}]", client_ip, req.method(), req.uri());

    let mut response = Response::new(Body::empty());

    match req.method() {
        &Method::GET => {
            *response.body_mut() = handle_embedded_file(req.uri().path());
        },
        &Method::POST => {
            match req.uri().path() {
                "/api/login" => {
                    *response.body_mut() = user::login(req).await;
                },
                "/api/user" => {
                    *response.body_mut() = user::info(req);
                },
                "/api/box" => {
                    *response.body_mut() = mybox::process(req).await;
                },
                _ => {
                    *response.status_mut() = StatusCode::NOT_FOUND;
                },
            }
        },
        _ => {
            *response.status_mut() = StatusCode::NOT_FOUND;
        },
    }

    Ok(response)
}

#[tokio::main]
async fn main() {
    let addr = ([0, 0, 0, 0], 8000).into();

    let make_svc = make_service_fn(move |_conn: &AddrStream| {
        let client_ip = _conn.remote_addr().ip();
        let service = service_fn(move |req| http_handler(client_ip, req));
        async move { Ok::<_, Infallible>(service) }
    });

    let server = Server::bind(&addr).serve(make_svc);

    if let Err(e) = server.await {
        eprintln!("server error: {}", e);
    }

}
