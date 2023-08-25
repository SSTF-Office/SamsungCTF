
use std::sync::Arc;
use mlua::{LuaSerdeExt, SerializeOptions};

pub fn create_json_encode(l: &mlua::Lua) -> mlua::Result<mlua::Function> {
    l.create_function(|l, table: mlua::Value| {
        let value: serde_json::Value = l.from_value(table).unwrap();

        match serde_json::to_string(&value) {
            Ok(str) => Ok(str),
            Err(e) => mlua::Result::Err(mlua::Error::ExternalError(Arc::new(e))),
        }
    })
}

pub fn create_json_decode(l: &mlua::Lua) -> mlua::Result<mlua::Function> {
    l.create_function(
        |l, json: String| match serde_json::from_str::<serde_json::Value>(&json) {
            Ok(serde_value) => l.to_value_with(
                &serde_value,
                SerializeOptions::new().set_array_metatable(false),
            ),
            Err(e) => mlua::Result::Err(mlua::Error::ExternalError(Arc::new(e))),
        },
    )
}