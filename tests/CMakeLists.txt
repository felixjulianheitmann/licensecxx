cmake_minimum_required(VERSION 3.14)

project(lcxx_tests)

add_subdirectory(crypto)
add_subdirectory(identifiers)
add_subdirectory(lcxx)

add_custom_target(TESTS DEPENDS
    crypto_test
    identifiers_test
    lcxx_test
)
