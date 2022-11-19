cmake_minimum_required(VERSION 3.14)

project(lcxx_tests)
include(CTest)
include(GoogleTest)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(identifiers)
add_subdirectory(lcxx)
add_subdirectory(network)
add_subdirectory(openssl_wrappers)

add_custom_target(TESTS DEPENDS
    identifiers_test
    lcxx_test
    lcxx_network_test
    openssl_wrappers
)
