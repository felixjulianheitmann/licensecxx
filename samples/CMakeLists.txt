cmake_minimum_required(VERSION 3.14)

project(lcxx_tests)

add_subdirectory(file_based_server)
add_subdirectory(ident_utils)
add_subdirectory(license_generator)
add_subdirectory(license_verifier)

add_custom_target(SAMPLES DEPENDS file_based_server license_generator license_verifier ident_utils_samples)
