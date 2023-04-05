# Licensecxx

![Build & Test Status Ubuntu](https://github.com/felixjulianheitmann/licensecxx/actions/workflows/build-test-ubuntu.yml/badge.svg)

![Build & Test Status Windows](https://github.com/felixjulianheitmann/licensecxx/actions/workflows/build-test-windows.yml/badge.svg)

![Build & Test Status Mac](https://github.com/felixjulianheitmann/licensecxx/actions/workflows/build-test-mac.yml/badge.svg)


- [Licensecxx](#licensecxx)
  - [How to use licensecxx](#how-to-use-licensecxx)
    - [Prerequisites](#prerequisites)
    - [Building](#building)
    - [Generate a license](#generate-a-license)
    - [Verify a license](#verify-a-license)
    - [Generating a key pair](#generating-a-key-pair)
  - [License Features (and roadmap)](#license-features-and-roadmap)
  - [License](#license)

*Copy protection library targeting Linux, Windows and Mac* (currently only Linux supported)

This project is inspired by the [**licensecc**](https://github.com/open-license-manager/licensecc) project.

Protect your software by generating and checking against license files. Create a set of key value pairs as content for your file and sign them with a RSA private key. The licensed software can then read this license file and verify the signature with the respective public key.

Additionally to providing key value pairs, you can include hash values of machine or user dependant data. Features such as name of the operating system, the machine hostname, the maximum CPU frequency can be hashed and included in the license. This enables machine/user specific license distribution.

Licensecxx provides a modern C++ interface with the main focus on ease of use and minimal impact on the software that it's protecting.

Due to the early development stage, licensecxx currently only supports linux hosts. Most of the software should be os-agnostic, but hardware/os identifiers rely on the operatings system they run on. One of the primary goals will be to enable cross-platform support, as soon as I can get my hands on a Windows/Mac to test things on.

A list of the available features and the roadmap can be seen at [License Features](#license-features).

This repository is still under development. If you have experience errors or bugs, please file an issue on the GitHub issues page. If you want to contribute, pull requests are very welcome.

## How to use licensecxx

### Prerequisites

The library requires C++20 support.

Licensecxx relies on [nlohmann/json](https://github.com/nlohmann/json) and [OpenSSL](https://www.openssl.org/).
nlohmann/json is fetched through CMake-Fetchcontent during configure time and made available the library. Whether this is the preferrable way is still up to debate.

OpenSSL needs to be made available by the parent project/user. Licensecxx only calls `FindPackage(OpenSSL)`. If that cannot find the OpenSSL 1.1 library, the licensecxx cannot be built.

### Building

This project is supposed to be used as a library addition to your software. Licensecxx is CMake based and can be included as a submodule in your project. Including it as a subdirectory makes the licensecxx targets available to your project

```cmake
add_subdirectory(lcxx)

target_link_libraries(your-executable PUBLIC
    lcxx::lcxx
    # lcxx::identifiers
)
```

The `lcxx::identifiers` library is optional to produce/verify identifying hash codes of the target machine.

### Generate a license

A simple license generator looks like this:
```c++
#include <lcxx/lcxx.hpp>

int main()
{
    lcxx::license license;

    // Push optional data into the license file
    license.push_content( "some key", "some value" );
    license.push_content( "hardware", lcxx::identifiers::hardware().hash );
    license.push_content( "os", lcxx::identifiers::os().hash );

    auto key = lcxx::crypto::load_key( some_pem_private_key_rsa_string, lcxx::crypto::key_type::private_key );
    // alternatively, load key from file by providing the file PEM file path
    // auto key = lcxx::crypto::load_key( some_pem_private_key_rsa_path, lcxx::crypto::key_type::private_key );
    lcxx::to_json(license, "my-project-license.lic", key);
}
```

It prints a signed license json file to the file `my-project-license.lic`. This is similar to the sample [license_generator](samples/license_generator/main.cpp).

### Verify a license

A generated license file or string can be loaded and verified provided a public key. Additional hardware identifiers can be checked against the currently running hardware.

```c++
int main()
{
    auto key                  = lcxx::crypto::load_key( some_pem_pem_key_rsa_string, lcxx::crypto::key_type::public_key );
    auto [license, signature] = lcxx::from_json( std::filesystem::path("my-project-license.lic") );

    if ( !lcxx::verify_license( license, signature, key ) ) {
        std::cout << "This program is not licensed!" << std::endl;
        return -1;
    }

    if ( lcxx::identifiers::verify( hw_ident_strat::all, license.get( "hardware" ) ) ) {
        std::cout << "The hardware does not match the one licensed!" << std::endl;
        return -2;
    }

    return 0;
}
```

A similar sample is given in [license_verifier](samples/license_verifier/main.cpp)

Further samples are given in the [samples](samples) folder.

### Generating a key pair

To generate an RSA key pair that can be processed by licensecxx the following openssl commands can be used:
```
openssl genrsa -out /path/to/private_key.rsa 1024
openssl rsa -in /path/to/private_key.rsa -outform PEM -pubout -out /path/to/public_key
```

These command generate a private-public key pair. Enabling the CMake option `LCXX_GENERATE_KEYS` will an additional CMake interface library called `lcxx::key`.
Linking it will have CMake generate a key-pair from that through a simple python script two header files:
- `public_key.hpp`
- `private_key.hpp`

For that location of the generated private/public keys can be specified with the variables `LCXX_PRIVATE_KEY`/`LCXX_PUBLIC_KEY`. The key size defaults to 1024 but can be configured through `LCXX_KEY_SIZE`.

Finally, `LCXX_KEY_HEADER_DIR` defines the path where the generated header files should be located. It will automatically be part of the `lcxx::key` include directories.

Enabling `LCXX_GENERATE_KEYS` will require Python as a dependency for the header generation.

## License Features (and roadmap)

Currently, all features are only provided for Linux based systems unless marked otherwise.

- [x] include CPU features
- [x] include OS/user features
- [x] Provide proper online documentation
- [ ] Make CD/CI run through on Windows / Mac / Ubuntu
- [ ] provide hardware/os identifiers for Windows
- [ ] provide hardware/os identifiers for Mac
- [ ] Provide a conan binary
- [ ] Provide a vcpkg binary
- [ ] Provide CMake fetchcontent option
- [ ] Find a way to test hardware identifiers in CD/CI


## License
The project is donated to the community. It uses the [MIT license](LICENSE). 
