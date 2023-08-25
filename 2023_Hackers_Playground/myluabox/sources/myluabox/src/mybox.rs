use crate::jwt;

use std::time::Duration;
use std::process::{Command, Stdio};
use std::io::Write;

use hyper::{Body, Request};
use serde::{Serialize, Deserialize};
use serde_json::json;

use wait_timeout::ChildExt;


#[derive(Serialize, Deserialize)]
struct LuaCode {
    code: String,
}


static LUAAGENT_PATH: &'static str = "luaagent";


pub async fn process(req: Request<Body>) -> Body {
    match jwt::jwt_verify(req.headers()) {
        Ok(token_claims) => {
            if (token_claims.username == "admin") && (token_claims.level == 0x61646D21) {
                let body = req.into_body();
                let body_bytes = hyper::body::to_bytes(body).await.unwrap();
                let body_str = String::from_utf8(body_bytes.to_vec()).unwrap();
                let luacode: LuaCode = match serde_json::from_str(&body_str) {
                    Ok(v) => v,
                    Err(_) => {
                        let json_result = json!({
                            "status": "error",
                            "message": "json error"
                        });
                        return Body::from(json_result.to_string());
                    }
                };

                let code = luacode.code.to_string();
                if code.len() < 2048 {
                    println!("  [{}]", code);

                    let out_str = run(code);
                    let box_result = json!({
                        "status": "ok",
                        "message": out_str,
                    });
                    Body::from(box_result.to_string())

                } else {
                    let box_result = json!({
                        "status": "error",
                        "message": "too long",
                    });
                    Body::from(box_result.to_string())
                }

            } else {
                let box_result = json!({
                    "status": "error",
                    "message": "Access Denied",
                });
                Body::from(box_result.to_string())
            }
        }
        Err(e) => {
            let box_result = json!({
                "status": "error",
                "message": e.to_string(), 
            });
            Body::from(box_result.to_string())
        }
    }
}


pub fn run(code: String) -> String {
    let mut out_str:String = "".to_string();

    let code = code + "\n";

    let mut child = Command::new(LUAAGENT_PATH)
    .stdin(Stdio::piped())
    .stdout(Stdio::piped())
    .spawn().unwrap();

    let mut stdin = child.stdin.take().expect("Failed to open stdin");
    stdin.write_all(code.as_bytes()).expect("Failed to write to stdin");

    let wait_sec = Duration::from_secs(1);
    match child.wait_timeout(wait_sec).unwrap() {
        Some(_) => {
            let output = child.wait_with_output().unwrap();
            //println!("stdout {}", String::from_utf8_lossy(&output.stdout));
            //println!("stderr {}", String::from_utf8_lossy(&output.stderr));
            let output_stdout = String::from_utf8_lossy(&output.stdout);
            out_str.push_str(&output_stdout);
        }, 
        None => {
            child.kill().unwrap();
            out_str = "timeout".to_string();
        }
    };

    out_str
}

