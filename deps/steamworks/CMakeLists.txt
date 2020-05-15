if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sdk/public/steam/steam_api.json)
  add_library(steamworks STATIC IMPORTED GLOBAL)
  target_include_directories(steamworks INTERFACE sdk/public)
endif()
