mod envlib;
mod functions;
mod json;

use std::io::{self, BufRead, Write};
use mlua::{Lua};

fn main() -> io::Result<()> {
    let stdout = io::stdout();
    let mut out_handle = stdout.lock();
    let stdin = io::stdin();
    let mut in_handle = stdin.lock();

    let lua = Lua::new();

    let functions = functions::load_functions(&lua).unwrap();
    lua.globals().set("functions", functions).unwrap();

    let backup = lua.globals();
    lua.set_named_registry_value("backup", backup).unwrap();

    let env_builder = envlib::EnvLibSrc::get("init.lua").unwrap();
    let env = lua.load(env_builder.data.as_ref()).eval::<mlua::Table>().unwrap();
    lua.set_named_registry_value("env", env).unwrap();

    let mut line = String::new();
    in_handle.read_line(&mut line).unwrap();

    match functions::load(&lua, &line) {
        Ok(v) => {
            let output = v.iter()
            .map(|value| format!("{:?}", value))
            .collect::<Vec<_>>()
            .join("\t");
            out_handle.write_all(output.as_bytes()).unwrap();
        },
        Err(e) => {
            out_handle.write_all(e.to_string().as_bytes()).unwrap();
        },
    }

    Ok(())
}
