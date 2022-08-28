cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

SET(CMAKE_DISABLE_SOURCE_CHANGES OFF) #keys are generated in the source tree by default
SET(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

IF(NOT LCXX_RESOURCE_DIR)
	message(STATUS "You should define a variable LCXX_RESOURCE_DIR containing directory which you want public/private key to be generated to. "
		"By default they will be generated to the global binary dir (CMAKE_BINARY_DIR/licensecxx).")
	set(LCXX_RESOURCE_DIR "${CMAKE_BINARY_DIR}/licensecxx") 
ENDIF(NOT LCXX_RESOURCE_DIR)

project (licensecxx
			VERSION 2.1.0
			LANGUAGES CXX)

add_custom_target(generate_private_key
    COMMAND "openssl genrsa -out ${LCXX_PRIVATE_KEY} ${LCXX_KEY_SIZE}"
    COMMENT "openssl rsa -in ${LCXX_PRIVATE_KEY} -outform PEM -pubout -out ${LCXX_PUBLIC_KEY}"
    DEPENDS ${PROJECT_NAME}
)

add_subdirectory(${PROJECT_SOURCE_DIR}/modules/base)
add_subdirectory(${PROJECT_SOURCE_DIR}/modules/hw_identifier)
add_subdirectory(${PROJECT_SOURCE_DIR}/modules/locate)
add_subdirectory(${PROJECT_SOURCE_DIR}/modules/os)
add_subdirectory(${PROJECT_SOURCE_DIR}/modules/licensecxx)


SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# #In case the build system doesn't support binfmt misc
# IF( ( CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" ) AND CMAKE_CROSSCOMPILING AND NOT DEFINED CMAKE_CROSSCOMPILING_EMULATOR )
# 	SET(CMAKE_CROSSCOMPILING_EMULATOR "wine")	
# ENDIF()

SET (OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL COMPONENTS Crypto QUIET)
IF(OPENSSL_FOUND)
	MESSAGE(STATUS "Found openssl version ${OPENSSL_VERSION} -licensecc")
ENDIF(OPENSSL_FOUND)

if(UNIX) #this is true for all the linux systems but not for cross compiling "linux->windows"
	IF(NOT OPENSSL_FOUND)
		MESSAGE(SEND_ERROR "Openssl required in Linux, please install it or specify -DOPENSSL_ROOT")
	ENDIF(NOT OPENSSL_FOUND)
	
    # find_package(Threads)
	# if(THREADS_HAVE_PTHREAD_ARG)
  	# 	list(APPEND EXTERNAL_LIBS "-pthread")
	# else(THREADS_HAVE_PTHREAD_ARG)
	# 	if(CMAKE_THREAD_LIBS_INIT)
  	# 		 list(APPEND EXTERNAL_LIBS "${CMAKE_THREAD_LIBS_INIT}")
	# 	endif()
	# endif(THREADS_HAVE_PTHREAD_ARG)

else(UNIX)
	IF(NOT OPENSSL_FOUND)
		MESSAGE(STATUS "Openssl not found, configuring the library without it (running in wine not supported).")
	ENDIF(NOT OPENSSL_FOUND)
	if(MSVC)
		# include(target_arch)
		# target_architecture( TARGET_ARCHITECTURE )
		# message(STATUS "architecture detected: ${TARGET_ARCHITECTURE}")
	    # list(APPEND EXTERNAL_LIBS "bcrypt" "crypt32" "ws2_32" "iphlpapi")
	else(MSVC)
	    # if(MINGW)        
        #     list(APPEND EXTERNAL_LIBS "-lbcrypt" "-lcrypt32" "-lws2_32" "-liphlpapi")
	    #     SET(CMAKE_EXE_LINKER_FLAGS "-static -static-libstdc++")
	    # endif(MINGW)
	endif(MSVC)
endif(UNIX)

#initialize project
#load the current project from files or find it from environment variables or create a default one
set(LCXX_INCLUDE_DIR "${LCXX_RESOURCE_DIR}/include" )
set(LCXX_PUBLIC_KEY "${LCC_INCLUDE_DIR}/public_key.h" )
set(LCXX_PRIVATE_KEY "${LCXX_RESOURCE_DIR}/private_key.rsa" )

IF(NOT LCXX_KEY_SIZE)
    set(LCXX_KEY_SIZE 2048)
ENDIF()

include_directories( ${LCC_INCLUDE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/include" )

#################################
# Documentation
#################################
find_package(Doxygen COMPONENTS dot)
find_package(Sphinx)
 
IF(Doxygen_FOUND AND Sphinx_FOUND)
    message(STATUS "Doxygen found, generating docs.")
    SET(DOXYGEN_EXCLUDE_PATTERNS "*/library/ini/*;*/doc/resources/*")
    SET(DOXYGEN_DISABLE_INDEX YES)
    #SET(DOXYGEN_GENERATE_TREEVIEW YES)
    SET(DOXYGEN_GENERATE_XML YES)
    SET(DOXYGEN_LAYOUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/doc/DoxygenLayout.xml")
	doxygen_add_docs(docs doc src include/lincesecxx ${LCXX_INCLUDE_DIR} COMMENT "doxygen docs")
	#file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/doc/resources/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/html")
	
	#file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
	set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/doc)
	set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/docs/sphinx)
 
	add_custom_target(documentation
                  COMMAND
                  ${SPHINX_EXECUTABLE} -b html
                  ${SPHINX_SOURCE} ${SPHINX_BUILD}
                  -Dbreathe_projects.licensecc=${CMAKE_CURRENT_BINARY_DIR}/xml
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  COMMENT "Generating documentation with Sphinx")
    add_dependencies(documentation docs)
    
ELSE(Doxygen_FOUND AND Sphinx_FOUND)
	message(STATUS "Doxygen or Sphynx not found, not generating docs.")
ENDIF(Doxygen_FOUND AND Sphinx_FOUND)

IF (NOT LCC_PROJECT_MAGIC_NUM) 
	set(LCC_PROJECT_MAGIC_NUM 0)  
ENDIF (NOT LCC_PROJECT_MAGIC_NUM)

IF(NOT EXISTS "${LCC_INCLUDE_DIR}/licensecc_properties.h")
	configure_file ("${PROJECT_SOURCE_DIR}/templates/licensecc_properties.h.in" 
		"${LCC_INCLUDE_DIR}/licensecc_properties.h")
ENDIF(NOT EXISTS "${LCC_INCLUDE_DIR}/licensecc_properties.h")
