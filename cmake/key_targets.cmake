
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
    set(LCXX_KEY_HEADER_DIR "${PROJECT_BUILD_DIR}/keys" )
endif(NOT LCXX_KEY_HEADER_DIR)

IF (NOT LCXX_PROJECT_MAGIC_NUM) 
	set(LCXX_PROJECT_MAGIC_NUM 0)  
ENDIF (NOT LCXX_PROJECT_MAGIC_NUM)

if(LCXX_GENERATE_KEY)
    add_custom_target(generate_private_key
        COMMAND "openssl genrsa -out ${LCXX_PRIVATE_KEY} ${LCXX_KEY_SIZE}"
        DEPENDS ${PROJECT_NAME}
    )
    add_custom_target(generate_public_key
        COMMAND "openssl rsa -in ${LCXX_PRIVATE_KEY} -outform PEM -pubout -out ${LCXX_PUBLIC_KEY}"
        DEPENDS ${PROJECT_NAME}
    )
endif(LCXX_GENERATE_KEY)

find_package(Python REQUIRED)

add_custom_target(generate_headers
    COMMAND "${Python_EXECUTABLE}" "${PROJECT_SOURE_DIR}/scripts/generate_headers.py" "${LCXX_PRIVATE_KEY}" "${LCXX_PUBLIC_KEY}" ${LCXX_KEY_SIZE} ${LCXX_KEY_HEADER_DIR}
)

add_library(lcxx_key_headers INTERFACE)
target_include_directories(lcxx_key_headers INTERFACE
    ${LCXX_KEY_HEADER_DIR}
)
add_library(lcxx::key ALIAS lcxx_key_headers)