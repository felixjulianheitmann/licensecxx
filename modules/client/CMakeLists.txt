project(lcxx_client_lib)

find_package(Boost 1.78.0 COMPONENTS headers)

if (Boost_FOUND)

    add_library(${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/src/client.cpp
    )

    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

    target_link_libraries(${PROJECT_NAME} PUBLIC
        lcxx::lcxx
        lcxx_net_common
        Boost::headers
        Boost::url
        ssl
    )

    target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    )

    add_library(lcxx::client ALIAS ${PROJECT_NAME})

else (Boost_FOUND)
    message("Boost not found - licensecxx client module will not be built")
endif (Boost_FOUND)
