enable_testing()

add_executable(crypto_test
    ${CMAKE_CURRENT_SOURCE_DIR}/crypto_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hash_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base64_test.cpp
)
target_link_libraries(crypto_test
    GTest::gtest_main
    lcxx::crypto
)

target_include_directories(crypto_test PRIVATE
    ${PROJECT_SOURCE_DIR}/common
)

target_compile_definitions(crypto_test PRIVATE 
    TEST_RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../resources"
)

include(GoogleTest)
gtest_discover_tests(crypto_test)