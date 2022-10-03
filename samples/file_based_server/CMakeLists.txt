add_executable(file_based_server
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(file_based_server
    lcxx::server
)


configure_file(${PROJECT_SOURCE_DIR}/resources/private_key.rsa ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/private_key.rsa COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/resources/public_key ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/public_key COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/resources/licenses/mylicense.json ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/licenses/mylicense.json COPYONLY)