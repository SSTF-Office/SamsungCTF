use thiserror::Error;


#[derive(Error, Debug)]
pub enum CustomError {
    #[error("jwt token not valid")]
    JWTTokenError,
    #[error("no auth header")]
    NoAuthHeaderError,
    #[error("invalid auth header")]
    InvalidAuthHeaderError,
}

