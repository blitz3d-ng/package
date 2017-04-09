local RUNTIME_CONFIG = dofile('./default.lua')
filter {}

table.insert(RUNTIME_CONFIG.modules, 'multiplayer')
removeplatforms { "macos", "linux" }

return RUNTIME_CONFIG
