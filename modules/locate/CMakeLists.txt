add_library( locate
    ApplicationFolder.cpp
    EnvironmentVarLocation.cpp
    EnvironmentVarData.cpp
    ExternalDefinition.cpp
    LocatorStrategy.cpp
    LocatorFactory.cpp
)

target_include_directories( locate PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries( locate PUBLIC
    lcxx::lcxx
    base
    os
)
