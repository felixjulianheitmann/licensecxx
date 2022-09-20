add_library(lcxx_license INTERFACE)

target_include_directories(lcxx_license INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(lcxx_license INTERFACE cxx_std_20)

add_library( lcxx::license ALIAS lcxx_license )

target_link_libraries( lcxx_license INTERFACE
    nlohmann_json::nlohmann_json
)
