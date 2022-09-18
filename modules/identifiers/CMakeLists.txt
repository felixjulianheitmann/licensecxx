add_library(lcxx_identifiers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/user.cpp
)

target_link_libraries( lcxx_identifiers PUBLIC
    lcxx::os_utils
)

target_include_directories( lcxx_identifiers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(lcxx::identifiers ALIAS lcxx_identifiers)