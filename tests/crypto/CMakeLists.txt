enable_testing()

add_executable(crypto_test
    ${CMAKE_CURRENT_SOURCE_DIR}/hash_test.cpp
)
target_link_libraries(crypto_test
    GTest::gtest_main
    lcxx::crypto
)

include(GoogleTest)
gtest_discover_tests(crypto_test)