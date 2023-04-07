add_library(lcxx_identifiers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/os.cpp
)

target_link_libraries( lcxx_identifiers PUBLIC
    lcxx::ident_utils
    lcxx::license
    lcxx::crypto
    nlohmann_json::nlohmann_json
)

target_include_directories( lcxx_identifiers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${VERSION_DIR}
)

target_compile_features(lcxx_identifiers PUBLIC cxx_std_20)
set_target_properties(lcxx_identifiers PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(lcxx::identifiers ALIAS lcxx_identifiers)