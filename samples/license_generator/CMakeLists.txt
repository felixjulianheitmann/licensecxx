add_executable(license_generator
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(license_generator
    lcxx::lcxx
    lcxx::identifiers
)
