How to use licensecxx
=====================


Prerequisites
-------------

Licensecxx relies on `nlohmann/json<https://github.com/nlohmann/json>`_ and `OpenSSL<https://www.openssl.org>`_.
nlohmann/json is fetched through CMake-Fetchcontent during configure time and made available the library. Whether this is the preferrable way is still up to debate.

OpenSSL needs to be made available by the parent project/user. Licensecxx only calls ``FindPackage(OpenSSL)``. If that cannot find the OpenSSL 1.1 library, the licensecxx cannot be built.

Building
--------

This project is supposed to be used as a library addition to your software. Licensecxx is CMake based and can be included as a submodule in your project. Including it as a subdirectory makes the licensecxx targets available to your project

.. code-block::cmake
    add_subdirectory(lcxx)

    target_link_libraries(your-executable PUBLIC
        lcxx::lcxx
        # lcxx::identifiers
    )

The ``lcxx::identifiers`` library is optional to produce/verify identifying hash codes of the target machine.

Generate a license
------------------

A simple license generator looks like this:
.. code-blocks::c++
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


It prints a signed license json file to the file ``my-project-license.lic``. This is similar to the sample [license_generator](samples/license_generator/main.cpp).

Verify a license
----------------

A generated license file or string can be loaded and verified provided a public key. Additional hardware identifiers can be checked against the currently running hardware.

.. code-blocks::c++
    int main()
    {
        auto key                  = lcxx::crypto::load_key( some_pem_pem_key_rsa_string, lcxx::crypto::key_type::public_key );
        auto [license, signature] = lcxx::from_json( std::filesystem::path("my-project-license.lic") );

        if ( !lcxx::verify_license( license, signature, key ) ) {
            std::cout << "This program is not licensed!" << std::endl;
            return -1;
        }

        if ( lcxx::identifiers::verify( license.get( "hardware" ) ) ) {
            std::cout << "The hardware does not match the one licensed!" << std::endl;
            return -2;
        }

        return 0;
    }

A similar sample is given in `license_verifier<samples/license_verifier/main.cpp>`_

Further samples are given in the `samples<samples>`_ folder.

Generating a key pair
---------------------

To generate an RSA key pair that can be processed by licensecxx the following openssl commands can be used:

.. code-block::
    openssl genrsa -out /path/to/private_key.rsa 1024
    openssl rsa -in /path/to/private_key.rsa -outform PEM -pubout -out /path/to/public_key


These command generate a private-public key pair. Enabling the CMake option ``LCXX_GENERATE_KEYS`` will an additional CMake interface library called ``lcxx::key``.
Linking it will have CMake generate a key-pair from that through a simple python script two header files:
- ``public_key.hpp``
- ``private_key.hpp``

For that location of the generated private/public keys can be specified with the variables ``LCXX_PRIVATE_KEY`` / ``LCXX_PUBLIC_KEY``. The key size defaults to 1024 but can be configured through ``LCXX_KEY_SIZE``.

Finally, ``LCXX_KEY_HEADER_DIR`` defines the path where the generated header files should be located. It will automatically be part of the ``lcxx::key`` include directories.

Enabling ``LCXX_GENERATE_KEYS`` will require Python as a dependency for the header generation.
