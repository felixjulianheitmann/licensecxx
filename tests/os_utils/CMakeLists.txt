enable_testing()

add_executable(os_utils_test
    ${CMAKE_CURRENT_SOURCE_DIR}/os_utils_test.cpp
)
target_link_libraries(os_utils_test
    GTest::gtest_main
    lcxx::os_utils
)

include(GoogleTest)
gtest_discover_tests(os_utils_test)