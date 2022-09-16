add_library( locate ApplicationFolder.cpp EnvironmentVarLocation.cpp EnvironmentVarData.cpp ExternalDefinition
                 .cpp LocatorStrategy.cpp LocatorFactory.cpp )

    target_include_directories( locate PUBLIC ${ CMAKE_CURRENT_SOURCE_DIR } )

        target_link_libraries( locate PUBLIC lcxx::lcxx base os )

            if ( CODE_COVERAGE AND UNIX ) target_compile_options( locate PUBLIC - O0 - g-- coverage )
                endif( CODE_COVERAGE AND UNIX )