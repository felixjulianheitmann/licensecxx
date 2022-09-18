add_executable(license_verifier
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(license_verifier
    lcxx::lcxx
)
