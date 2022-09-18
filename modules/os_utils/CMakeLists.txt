add_library(lcxx_os_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/disk_utils.cpp
)

target_include_directories(lcxx_os_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(lcxx::os_utils ALIAS lcxx_os_utils)