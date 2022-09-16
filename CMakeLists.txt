cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

SET(CMAKE_DISABLE_SOURCE_CHANGES OFF) #keys are generated in the source tree by default
SET(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

IF(NOT LCXX_RESOURCE_DIR)
	message(STATUS "You should define a variable LCXX_RESOURCE_DIR containing the directory where public/private key are located or where they should be generated to. "
		"By default they will be generated to the global binary dir (CMAKE_BINARY_DIR/project_keys).
        If they are provided: place them as ")
	set(LCXX_RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources/project_keys") 
ENDIF(NOT LCXX_RESOURCE_DIR)

option(LCXX_GENERATE_KEYS "Whether RSA key files should be generated during build time." ON)

project (lcxx
			VERSION 0.1.0
			LANGUAGES CXX)

include(${PROJECT_SOURCE_DIR}/cmake/key_targets.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/get_nlohmann_json.cmake)

# add_subdirectory(${PROJECT_SOURCE_DIR}/modules/base)
# add_subdirectory(${PROJECT_SOURCE_DIR}/modules/hw_identifier)
# add_subdirectory(${PROJECT_SOURCE_DIR}/modules/locate)
# add_subdirectory(${PROJECT_SOURCE_DIR}/modules/os)
add_subdirectory(${PROJECT_SOURCE_DIR}/modules/lcxx)

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

