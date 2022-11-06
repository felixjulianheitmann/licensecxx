enable_testing()

add_executable(lcxx_network_test
    ${CMAKE_CURRENT_SOURCE_DIR}/lcxx_network_test.cpp
)
target_link_libraries(lcxx_network_test
    GTest::gtest_main
    lcxx::server
    lcxx::client
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../resources/certificate.pem ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/certificate.pem COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../resources/key.pem ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/key.pem COPYONLY)

gtest_discover_tests(lcxx_network_test)
