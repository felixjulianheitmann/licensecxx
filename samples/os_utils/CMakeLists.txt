add_executable(os_utils_samples
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(os_utils_samples
    lcxx::os_utils
    nlohmann_json::nlohmann_json
)
