use rust_embed::RustEmbed;

#[derive(RustEmbed)]
#[folder = "$CARGO_MANIFEST_DIR/resources"]
pub struct EnvLibSrc;

fn lua_name_to_path(path: &str) -> String {
    let mut path = path.replace(".", "/");
    path.push_str(".lua");
    path
}

pub fn require(l: &mlua::Lua) -> mlua::Result<mlua::Function> {
    l.create_function(|l, name : String| {
        let filename = lua_name_to_path(&name);
        let envlibmod = l.named_registry_value::<_, mlua::Value>(&name);
        if !matches!(envlibmod, Ok(mlua::Value::Nil)) {
            Ok(envlibmod)
        } else {
            if let Some(src) = EnvLibSrc::get(&filename) {
                let envlibmod = l.load(src.data.as_ref()).set_name(&name)?.eval::<mlua::Value>()?;
                l.set_named_registry_value(&name, envlibmod)?;
                let envlibmod = l.named_registry_value::<_, mlua::Value>(&name)?;
                Ok(Ok(envlibmod))
            } else {
                println!("file not found {}", filename);
                Ok(Ok(mlua::Value::Nil))
            }
        }
    })
}