add_library(hw_identifier
    hw_identifier_facade.cpp
    default_strategy.cpp
    ethernet.cpp
    disk_strategy.cpp
    identification_strategy.cpp
    hw_identifier.cpp
)

target_link_libraries( hw_identifier PUBLIC
    lcxx::lcxx
    os
    base
)

target_include_directories( hw_identifier PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)