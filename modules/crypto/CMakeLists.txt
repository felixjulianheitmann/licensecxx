add_library(lcxx_crypto
    ${CMAKE_CURRENT_SOURCE_DIR}/src/crypto.cpp
)

find_package(OpenSSL COMPONENTS Crypto)

target_link_libraries( lcxx_crypto PUBLIC
    OpenSSL::Crypto
)

target_include_directories(lcxx_crypto PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(lcxx::crypto ALIAS lcxx_crypto)