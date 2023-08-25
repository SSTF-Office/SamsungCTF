use crate::{envlib, json};

pub fn load<'lua>(l: &'lua mlua::Lua, code: &str) -> mlua::Result<mlua::MultiValue<'lua>> {
    let sandbox_env: mlua::Value = l.named_registry_value("env")?;
    l.load(code)
    .set_environment(sandbox_env).unwrap()
    .eval::<mlua::MultiValue>()
}

pub fn load_functions(l: &mlua::Lua) -> mlua::Result<mlua::Table> {
    let t = l.create_table()?;

    t.set(
        "random_seed",
        l.create_function(|_, ()| {
            use rand::Rng;
            let v: u64 = rand::thread_rng().gen();
            Ok(v)
        })?,
    )?;

    t.set(
        "load",
        l.create_function(|l, code : String| {
            load(l, &code)
        })?,
    )?;

    t.set(
        "require",
        envlib::require(l)?
    )?;

    t.set("json_encode", json::create_json_encode(l)?)?;
    t.set("json_decode", json::create_json_decode(l)?)?;

    Ok(t)
}