project(lcxx_server_lib)

find_package(Boost 1.78.0 COMPONENTS headers)

if (Boost_FOUND)

    add_library(${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/src/server.cpp
    )

    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

    target_link_libraries(${PROJECT_NAME} PUBLIC
        lcxx::lcxx
        lcxx_net_common
        Boost::headers
        ssl
    )

    target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    )

    add_library(lcxx::server ALIAS ${PROJECT_NAME})

else (Boost_FOUND)
    message("Boost not found - licensecxx server module will not be built")
endif (Boost_FOUND)
