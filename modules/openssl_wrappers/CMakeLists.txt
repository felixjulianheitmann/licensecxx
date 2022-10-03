add_library(lcxx_openssl_wrappers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/aes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rsa.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/encoding.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hash.cpp
)

find_package(OpenSSL COMPONENTS Crypto REQUIRED)

target_compile_features(lcxx_openssl_wrappers PUBLIC cxx_std_20)
set_target_properties(lcxx_openssl_wrappers PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)

target_link_libraries( lcxx_openssl_wrappers PUBLIC
    OpenSSL::Crypto
)

target_include_directories(lcxx_openssl_wrappers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(lcxx::openssl_wrappers ALIAS lcxx_openssl_wrappers)