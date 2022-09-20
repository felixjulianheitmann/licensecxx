add_executable(ident_utils_samples
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(ident_utils_samples PRIVATE
    lcxx::ident_utils
    nlohmann_json::nlohmann_json
)
