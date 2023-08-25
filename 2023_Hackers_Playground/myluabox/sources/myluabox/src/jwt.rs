
use crate::errors;

use serde::{Serialize, Deserialize};
use hyper::header::{AUTHORIZATION};
use hyper::{HeaderMap};

use jsonwebtoken::{encode, decode, Algorithm, Header, EncodingKey, DecodingKey, Validation};


#[derive(Serialize, Deserialize)]
pub struct Claims {
    pub username: String,
    pub level: u32,
    pub exp: usize,
}

static JWT_KEY: &'static str = "th1s1sjw7k3y";

pub fn jwt_encode(username: String, level: u32) -> std::string::String {
    let my_claims = Claims {
        username: username,
        level: level,
        exp: 10000000000
    };

    match encode(&Header::default(), &my_claims, &EncodingKey::from_secret(JWT_KEY.as_ref())) {
        Ok(v) => return v,
        Err(_) => return "".to_string(),
    }
}

fn jwt_from_header(headers: &HeaderMap) -> std::result::Result<String, errors::CustomError> {
    let header = match headers.get(AUTHORIZATION) {
        Some(v) => v,
        None => return Err(errors::CustomError::NoAuthHeaderError),
    };

    let auth_header = match std::str::from_utf8(header.as_bytes()) {
        Ok(v) => v,
        Err(_) => return Err(errors::CustomError::NoAuthHeaderError),
    };

    if !auth_header.starts_with("Bearer ") {
        return Err(errors::CustomError::InvalidAuthHeaderError);
    }
    Ok(auth_header.trim_start_matches("Bearer ").to_owned())
}

pub fn jwt_verify(headers: &HeaderMap) -> std::result::Result<Claims, errors::CustomError> {
    match jwt_from_header(headers) {
        Ok(token) => {
            let token_data = match decode::<Claims>(&token, &DecodingKey::from_secret(JWT_KEY.as_ref()), &Validation::new(Algorithm::HS256)) {
                Ok(c) => c,
                Err(_) => return Err(errors::CustomError::JWTTokenError),
            };
            Ok(token_data.claims)
        }
        Err(e) => return Err(e),
    }
}


