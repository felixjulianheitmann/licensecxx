enable_testing()

add_executable(lcxx_test
    ${CMAKE_CURRENT_SOURCE_DIR}/lcxx_test.cpp
)
target_link_libraries(lcxx_test
    GTest::gtest_main
    lcxx::lcxx
)

target_include_directories(lcxx_test PRIVATE
    ${PROJECT_SOURCE_DIR}/common
)

gtest_discover_tests(lcxx_test)
