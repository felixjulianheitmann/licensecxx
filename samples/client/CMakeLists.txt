add_executable(client_sample
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(client_sample
    lcxx::lcxx
    lcxx::client
)

configure_file(${PROJECT_SOURCE_DIR}/resources/public_key ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/public_key COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/resources/signature_pub_key ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/signature_pub_key COPYONLY)
