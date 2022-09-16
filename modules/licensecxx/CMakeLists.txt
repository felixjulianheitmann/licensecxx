add_library( licensecxx STATIC ${ CMAKE_CURRENT_SOURCE_DIR } / src / licensecxx.cpp ${ CMAKE_CURRENT_SOURCE_DIR } /
             src / LicenseReader.cpp ${ CMAKE_CURRENT_SOURCE_DIR } / src / limits /
             license_verifier.cpp    ${ CMAKE_CURRENT_SOURCE_DIR } / src / ini / ConvertUTF.cpp )

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

    target_include_directories(
        licensecxx PUBLIC $< INSTALL_INTERFACE : include > $< INSTALL_INTERFACE : include / ${ LCXX_RESOURCE_DIR } >
                $< BUILD_INTERFACE : ${ CMAKE_CURRENT_SOURCE_DIR } / include >
                $< BUILD_INTERFACE : ${ LCXX_INCLUDE_DIR } > )

        add_library( lcxx::lcxx ALIAS licensecxx )

            target_link_libraries( licensecxx PRIVATE base os hw_identifier locate )

#install( TARGETS licensecxx EXPORT licensecxx
#ARCHIVE DESTINATION ${ main_lib_dest } / ${ LCXX_RESOURCE_DIR } )

#install( EXPORT licensecxx NAMESPACE licensecxx::DESTINATION ${ main_lib_dest } / ${ LCXX_RESOURCE_DIR } / cmake )
#export( EXPORT licensecxx NAMESPACE licensecxx::FILE ${ CMAKE_BINARY_DIR } / ${ PROJECT_NAME }.cmake )
