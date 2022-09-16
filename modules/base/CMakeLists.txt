ADD_LIBRARY( base
    EventRegistry.cpp
    file_utils.cpp
    string_utils.cpp
    base64.cpp
    logger.cpp
)

target_include_directories( base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries( base PUBLIC
    lcxx::lcxx
)