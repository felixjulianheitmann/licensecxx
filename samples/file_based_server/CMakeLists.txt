add_executable(file_based_server
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(file_based_server
    lcxx::server
)
