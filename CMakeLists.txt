cmake_minimum_required(VERSION 3.20...4.5)

SET(CMAKE_DISABLE_SOURCE_CHANGES OFF) #keys are generated in the source tree by default
SET(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

option(LCXX_GENERATE_KEYS "Whether RSA key files should be generated during build time." OFF)
option(LCXX_BUILD_TESTING "Whether to build tests." OFF)
option(LCXX_BUILD_SAMPLES "Whether to build samples." OFF)

include(cmake/get_version.cmake)
pull_version()

project (lcxx
			VERSION ${GIT_VER_BASE}
			LANGUAGES CXX)

find_package(nlohmann_json REQUIRED)

set(VERSION_DIR ${CMAKE_BINARY_DIR}/version)
configure_file("scripts/version.hpp.in" "${CMAKE_BINARY_DIR}/version/lcxx/version.hpp")

include(${PROJECT_SOURCE_DIR}/cmake/key_targets.cmake)

add_subdirectory(${PROJECT_SOURCE_DIR}/modules)

if(LCXX_BUILD_TESTING)
	find_package(GTest REQUIRED)
	enable_testing()
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif()

if(LCXX_BUILD_SAMPLES)
    add_subdirectory(${PROJECT_SOURCE_DIR}/samples)
endif()
