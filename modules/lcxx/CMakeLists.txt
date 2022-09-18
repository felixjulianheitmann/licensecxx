add_library( licensecxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/writer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/reader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/verifier.cpp    
)

#define_property( TARGET PROPERTY WITH_OPENSSL BRIEF_DOCS "need openssl to compile" FULL_DOCS "ff" )
#IF( UNIX OR OPENSSL_FOUND )
#target_compile_definitions( licensecc_static PUBLIC HAS_OPENSSL )
#set_target_properties( licensecc_static PROPERTIES WITH_OPENSSL 1 )
#target_link_libraries( licensecc_static PUBLIC OpenSSL::Crypto ${ EXTERNAL_LIBS } ${ CMAKE_DL_LIBS } )
#ELSE( UNIX OR OPENSSL_FOUND )
#set_target_properties( licensecc_static PROPERTIES WITH_OPENSSL 0 )
#target_link_libraries( licensecc_static PUBLIC ${ EXTERNAL_LIBS } )
#ENDIF( UNIX OR OPENSSL_FOUND )

#set_property( TARGET licensecc_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )

target_include_directories(licensecxx PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(licensecxx PUBLIC cxx_std_20)

add_library( lcxx::lcxx ALIAS licensecxx )

target_link_libraries( licensecxx PUBLIC
    nlohmann_json::nlohmann_json
    lcxx::crypto
)
