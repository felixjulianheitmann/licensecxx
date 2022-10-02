add_executable(file_based_server
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(file_based_server
    lcxx::server
)


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/text_file.txt ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/text_file.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/licenses/mylicense.json ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/licenses/mylicense.json COPYONLY)