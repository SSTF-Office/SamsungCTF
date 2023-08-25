local json = {}

function json.encode(val, ...) return functions.json_encode(val) end
function json.decode(val, ...) return functions.json_decode(val) end

return json