# add_subdirectory(src)

# make this work...
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE src/single_include/nlohmann)
