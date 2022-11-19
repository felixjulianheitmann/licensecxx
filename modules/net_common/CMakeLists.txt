
add_library(lcxx_net_common INTERFACE)

target_include_directories(lcxx_net_common INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(Boost 1.78.0 COMPONENTS REQUIRED)
target_link_libraries( lcxx_net_common INTERFACE
    nlohmann_json::nlohmann_json
    Boost::headers
)

add_library( lcxx::net_common ALIAS lcxx_net_common )


