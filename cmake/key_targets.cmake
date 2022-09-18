
# Optionally have the key files be generated during build time if NOT already present
if(LCXX_GENERATE_KEYS)

    IF(NOT LCXX_KEY_SIZE)
        set(LCXX_KEY_SIZE 2048)
    ENDIF()

    if(NOT LCXX_PRIVATE_KEY)
        set(LCXX_PRIVATE_KEY "${LCXX_RESOURCE_DIR}/private_key.rsa" )
    endif(NOT LCXX_PRIVATE_KEY)

    if(NOT LCXX_PUBLIC_KEY)
        set(LCXX_PUBLIC_KEY "${LCXX_RESOURCE_DIR}/public_key" )
    endif(NOT LCXX_PUBLIC_KEY)

    if(NOT LCXX_KEY_HEADER_DIR)
        set(LCXX_KEY_HEADER_DIR "${PROJECT_BINARY_DIR}/keys/include" )
    endif(NOT LCXX_KEY_HEADER_DIR)

    IF (NOT LCXX_PROJECT_MAGIC_NUM) 
        set(LCXX_PROJECT_MAGIC_NUM 0)  
    ENDIF (NOT LCXX_PROJECT_MAGIC_NUM)

    IF(NOT LCXX_RESOURCE_DIR)
        message(STATUS "You should define a variable LCXX_RESOURCE_DIR containing the directory where public/private key are located or where they should be generated to. "
            "By default they will be generated to the global binary dir (CMAKE_BINARY_DIR/project_keys).
            If they are provided: place them as ")
        set(LCXX_RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources/project_keys") 
    ENDIF(NOT LCXX_RESOURCE_DIR)

    find_package(Python REQUIRED)

    add_custom_target(generate_headers
        COMMAND "${Python_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/generate_headers.py" "${LCXX_PRIVATE_KEY}" "${LCXX_PUBLIC_KEY}" ${LCXX_KEY_HEADER_DIR}
    )

    add_library(lcxx_key_headers INTERFACE)
    target_include_directories(lcxx_key_headers INTERFACE
        ${LCXX_KEY_HEADER_DIR}
    )
    add_dependencies(lcxx_key_headers generate_headers)
    add_library(lcxx::key ALIAS lcxx_key_headers)

    if(NOT EXISTS ${LCXX_PRIVATE_KEY})
        cmake_path(GET LCXX_PRIVATE_KEY PARENT_PATH LCXX_PRIVATE_KEY_DIR)
        file(MAKE_DIRECTORY ${LCXX_PRIVATE_KEY_DIR})

        add_custom_target(generate_private_key
        COMMAND "/usr/bin/openssl" "genrsa" "-out" "${LCXX_PRIVATE_KEY}" "${LCXX_KEY_SIZE}"
        )
        add_dependencies(generate_headers generate_private_key)
    endif()

    if(NOT EXISTS ${LCXX_PUBLIC_KEY})
        cmake_path(GET LCXX_PUBLIC_KEY PARENT_PATH LCXX_PUBLIC_KEY_DIR)
        file(MAKE_DIRECTORY ${LCXX_PUBLIC_KEY_DIR})

        add_custom_target(generate_public_key
        COMMAND "/usr/bin/openssl" "rsa" "-in" "${LCXX_PRIVATE_KEY}" "-outform" "PEM" "-pubout" "-out" "${LCXX_PUBLIC_KEY}"
        )
        add_dependencies(generate_headers generate_public_key)
    endif()

endif(LCXX_GENERATE_KEYS)
