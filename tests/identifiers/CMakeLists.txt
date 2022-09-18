enable_testing()

add_executable(identifiers_test
    ${CMAKE_CURRENT_SOURCE_DIR}/identifiers_test.cpp
)
target_link_libraries(identifiers_test
    GTest::gtest_main
    lcxx::identifiers
)

include(GoogleTest)
gtest_discover_tests(identifiers_test)