ADD_LIBRARY(base
    EventRegistry.cpp
    file_utils.cpp
    string_utils.cpp
    base64.cpp
    logger.cpp
)

if(CODE_COVERAGE AND UNIX)
	  MESSAGE(STATUS "Enabling code coverage")
	  target_compile_options(base PUBLIC
	    -O0        # no optimization
	    -g         # generate debug info
	    --coverage # sets all required flags
	  )
endif(CODE_COVERAGE AND UNIX)

target_include_directories(base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(base PUBLIC
    lcxx::lcxx
)