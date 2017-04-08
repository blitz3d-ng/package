local RUNTIME_CONFIG = dofile('./default.lua')
table.insert(RUNTIME_CONFIG.modules, 'multiplayer')
return RUNTIME_CONFIG
