import os

from conan import ConanFile, Version
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, cmake_layout, CMakeToolchain
from conan.tools.build import check_min_cppstd


class LicenseCxxConan(ConanFile):
    name = "licensecxx"
    package_type = "library"


    url = "https://github.com/felixjulianheitmann/licensecxx"
    description = "LicenseCxx C++ library"
    topics = ("licensecxx", "library", "license")

    settings = "os", "compiler", "build_type"
    options = {"with_tests": [True, False], "shared": [True, False]}
    default_options = {"with_tests": False, "shared": False}

    generators = "CMakeDeps"

    exports_sources = "CMakeLists.txt", "modules/*/src/*", "modules/*/include/*"
    no_copy_source = True

    def validate(self):
        check_min_cppstd(self, "20")

    # def layout(self):
    #     cmake_layout(self)

    def requirements(self):
        self.requires("openssl/4.1.0")

    def build_requirements(self):
        if self.options.with_tests:
            self.test_requires("gtest/1.18.0")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if self.options.with_tests:
            cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()
