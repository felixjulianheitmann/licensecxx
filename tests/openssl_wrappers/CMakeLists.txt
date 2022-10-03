enable_testing()

add_executable(openssl_wrappers_test
    ${CMAKE_CURRENT_SOURCE_DIR}/aes_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rsa_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hash_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base64_test.cpp
)
target_link_libraries(openssl_wrappers_test
    GTest::gtest_main
    lcxx::openssl_wrappers
)

target_include_directories(openssl_wrappers_test PRIVATE
    ${PROJECT_SOURCE_DIR}/common
)

target_compile_definitions(openssl_wrappers_test PRIVATE 
    TEST_RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../resources"
)

include(GoogleTest)
gtest_discover_tests(openssl_wrappers_test)