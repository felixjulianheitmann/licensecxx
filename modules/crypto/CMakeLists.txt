add_library(lcxx_crypto
    ${CMAKE_CURRENT_SOURCE_DIR}/src/crypto.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/encoding.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hash.cpp
)

find_package(OpenSSL COMPONENTS Crypto REQUIRED)

target_compile_features(lcxx_crypto PUBLIC cxx_std_20)
set_target_properties(lcxx_crypto PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)

target_link_libraries( lcxx_crypto PUBLIC
    OpenSSL::Crypto
)

target_include_directories(lcxx_crypto PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${VERSION_DIR}
)

add_library(lcxx::crypto ALIAS lcxx_crypto)