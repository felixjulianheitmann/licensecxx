add_library(lcxx_ident_utils)

target_include_directories(lcxx_ident_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(lcxx_ident_utils PUBLIC cxx_std_20)
set_target_properties(lcxx_ident_utils PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    message("Configuring OS utilities for Windows")
    target_sources(lcxx_ident_utils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu_utils_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/os_utils_win.cpp
    )
    elseif(UNIX)
    if(APPLE)
    message("Configuring OS utilities for Mac")
    target_sources(lcxx_ident_utils PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu_utils_mac.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/os_utils_mac.cpp
        )
    else()
    message("Configuring OS utilities for Linux")
    target_sources(lcxx_ident_utils PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu_utils_linux.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/os_utils_linux.cpp
        )

    endif()
endif()

add_library(lcxx::ident_utils ALIAS lcxx_ident_utils)