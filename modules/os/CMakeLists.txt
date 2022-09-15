IF(UNIX OR OPENSSL_FOUND)
	IF(UNIX)
		add_library(os
		    openssl/signature_verifier.cpp
            execution_environment_common.cpp
		    linux/execution_environment.cpp
		    linux/cpu_info.cpp
		    linux/dmi_info.cpp
		    linux/network.cpp
		    linux/os_linux.cpp) 
	ELSE(UNIX)
	    #windows and openssl
  	    add_library(os 
  	    windows/cpu_info.cpp
  	    windows/dmi_info.cpp  
  	    openssl/signature_verifier.cpp 
        execution_environment_common.cpp 
        windows/execution_environment.cpp
  	    windows/os_win.cpp
  	    windows/network.cpp)
	ENDIF(UNIX)
	target_include_directories(os PUBLIC ${OPENSSL_INCLUDE_DIR})
ELSE(UNIX OR OPENSSL_FOUND)
#windows no openssl
    add_library(os
        windows/cpu_info.cpp
        windows/dmi_info.cpp 
        windows/signature_verifier.cpp 
        execution_environment_common.cpp 
        windows/execution_environment.cpp
        windows/os_win.cpp
        windows/network.cpp)
ENDIF(UNIX OR OPENSSL_FOUND)


target_include_directories(os PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
#dependency of signature_verifier.cpp
add_dependencies( os lcxx::key )

target_link_libraries(os PUBLIC
    lcxx::key
    base
)

if(CODE_COVERAGE AND UNIX)
	  target_compile_options(os PUBLIC -O0 -g --coverage)
endif(CODE_COVERAGE AND UNIX)