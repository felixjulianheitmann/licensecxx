cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

SET(CMAKE_DISABLE_SOURCE_CHANGES OFF) #keys are generated in the source tree by default
SET(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

option(LCXX_GENERATE_KEYS "Whether RSA key files should be generated during build time." OFF)

include(cmake/get_version.cmake)
pull_version()

project (lcxx
			VERSION ${GIT_VER_BASE}
			LANGUAGES CXX)

set(VERSION_DIR ${CMAKE_BINARY_DIR}/version)
configure_file("scripts/version.hpp.in" "${CMAKE_BINARY_DIR}/version/lcxx/version.hpp")

include(${PROJECT_SOURCE_DIR}/cmake/key_targets.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/get_nlohmann_json.cmake)

add_subdirectory(${PROJECT_SOURCE_DIR}/modules)

add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
add_subdirectory(${PROJECT_SOURCE_DIR}/samples)