use crate::jwt;

use hyper::{Body, Request};

use serde::{Serialize, Deserialize};
use serde_json::json;


#[derive(Serialize, Deserialize)]
struct UserLogin {
    username: String,
    password: String,
}


pub async fn login(req: Request<Body>) -> Body {
    let body = req.into_body();
    let body_bytes = hyper::body::to_bytes(body).await.unwrap();
    let body_str = String::from_utf8(body_bytes.to_vec()).unwrap();

    let requser: UserLogin = match serde_json::from_str(&body_str) {
        Ok(v) => v,
        Err(_) => {
            let json_result = json!({
                "status": "error",
                "message": "json error"
            });
            return Body::from(json_result.to_string());
        }
    };

    println!("  [{}] [{}]", requser.username, requser.password);

    if requser.username.to_ascii_lowercase() == "admin" {
        let login_result = json!({
            "status": "error",
            "message": "Access Denied"
        });
        
        Body::from(login_result.to_string())

    } else {
        let token = jwt::jwt_encode(requser.username, 1);
        //println!("  Token: {}", token);

        let login_result = json!({
            "status": "ok",
            "message": "Login Success",
            "accessToken": token
        });
        
        Body::from(login_result.to_string())
    }
}


pub fn info(req: Request<Body>) -> Body {
    match jwt::jwt_verify(req.headers()) {
        Ok(token_claims) => {
            let user_info = json!({
                "status": "ok",
                "username": token_claims.username,
            });
            return Body::from(user_info.to_string())
        },
        Err(e) => {
            let user_info = json!({
                "status": "error",
                "message": e.to_string(),
            });
            return Body::from(user_info.to_string())
        },
    };
}
