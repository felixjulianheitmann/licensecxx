add_library( licensecxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/writer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/reader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/verifier.cpp    
)

target_include_directories(licensecxx PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(licensecxx PUBLIC cxx_std_20)
set_target_properties(licensecxx PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)

add_library( lcxx::lcxx ALIAS licensecxx )

target_link_libraries( licensecxx PUBLIC
    nlohmann_json::nlohmann_json
    lcxx::crypto
    lcxx::license
)
