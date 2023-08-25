system = function() end
package = {
    loaded = {}
}
os = {
    clock = os.clock,
    date = os.date,
    difftime = os.difftime,
    time = os.time,
}
base = {
    tonumber = tonumber,
    tostring = tostring,
    type = type,
    ipairs = ipairs,
}
math = {
    randomseed = functions.random_seed,
}
io = {}
debug = {}
coroutine = {}
package.loaded["package"] = package
package.loaded["base"] = base
package.loaded["os"] = os
e = {
    _VERSION = _VERSION,
    package = package, 
    os = os,
    string = string,
    tostring = tostring,
    tonumber = tonumber,
    table = table,
    next = next,
    pairs = pairs,
    ipairs = ipairs,
    math = math,
    print = print,
    type = type,
    utf8 = utf8,
    load = functions.load,
    require = functions.require,
};

e["_G"] = e;
return e
