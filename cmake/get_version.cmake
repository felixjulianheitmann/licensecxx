cmake_minimum_required(VERSION 3.19)

function(pull_version)

execute_process(
    COMMAND git describe --tags
    WORKING_DIRECTORY "${CMAKE_PROJECT_DIR}"
    OUTPUT_VARIABLE GIT_TAG
    RESULTS_VARIABLE GIT_TAG_ERR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message("Git describe: ${GIT_TAG}")
string(REGEX MATCH "v.*\..*\..*" SEMVER_FOUND ${GIT_TAG})

if("${SEMVER_FOUND}" STREQUAL "")
    set(GIT_VER_SEM "v0.0.0-dev")
    set(GIT_VER_STR "0.0.0-dev")
    set(GIT_VER_MAJOR 0)
    set(GIT_VER_MINOR 0)
    set(GIT_VER_BUILD 0)
    set(GIT_VER_TAIL "dev")
endif()

string(REGEX MATCHALL "[0-9+]" VER_NUMBERS ${GIT_TAG})
message("Git describe: ${GIT_TAG}")
foreach(N ${VER_NUMBERS})
    message(${N})
endforeach()


string(FIND ${GIT_TAG} "." VER_MAJOR_END)
math(EXPR VER_MAJOR_END ${VER_MAJOR_END}-1)
string(SUBSTRING ${GIT_TAG} 1 ${VER_MAJOR_END} GIT_VER_MAJOR)
string(FIND ${GIT_TAG} "." VER_MINOR_END)
math(EXPR VER_MINOR_END ${VER_MINOR_END}-1)
string(SUBSTRING ${GIT_TAG} 1 ${VER_MINOR_END} GIT_VER_MAJOR)

message("GIT_VER_SEM: ${GIT_VER_SEM}")
message("GIT_VER_STR: ${GIT_VER_STR}")
message("GIT_VER_MAJOR: ${GIT_VER_MAJOR}")
message("GIT_VER_MINOR: ${GIT_VER_MINOR}")
message("GIT_VER_BUILD: ${GIT_VER_BUILD}")
message("GIT_VER_TAIL: ${GIT_VER_TAIL}")

endfunction()