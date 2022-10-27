.. _network_licensing_doc:

Network Licensing
=================

.. toctree::
   :maxdepth: 2
   :caption: Contents:


In many use cases the approach of providing a custom license file to the customer seems old-fashioned or too static.
A more modern, flexible approach is to have the application in question verify itself through a network server.

The application may collect user data such as username and password or domain information or anything which defines the extent of the execution permission.
This information can be serialized in form of a license and sent to a network server that signs this license using a private key.
The server can save license and signature as a form of registration.

During future execution attempts, the application can request that license and signature - or even just the signature - from the license server and
verify its execution permission.

To simplify such a workflow, Licensecxx includes a server-client library set. The `lcxx::server` CMake target features a minimalistic HTTP server with SSL/TLS encryption.
It's built on `boost::beast` which is therefore required for compilation. Due to the use of coroutines a compiler with C++20 support is also required.

Setting up the server can be as simple as:

.. code-block:: c++
    #include <lcxx/server.hpp>

    using namespace lcxx;

    auto main() -> int
    {
        server server( "0.0.0.0", 8080, "certificate.pem", "key.pem" );
        server.on_endpoint( "/licenses/*", [&]( net::dynamic_request const & req ) {
            // Code that will be executed on any request to an endpoint starting with /licenses
        }

        server.on_endpoint("/licenses/a_very_special_endpoint", [&]( net::dynamic_request const & req ) {
            // Specific endpoint matches take precedence over pattern matches
        }

        try {
            server.run( server::run_option::sync );
        }
        catch ( std::exception & e ) {
            server.stop();
            std::cout << "Error: " << e.what() << std::endl;
        }

        return 0;
    }

The `dynamic_request` is a `boost::beast::http::request< boost::beast::http::dynamic_body >`. It provides header and body data as well as the request method (`GET`/`POST`/etc.).

This construct can be easily used to store, load and delete licenses. A slightly more verbose example of a file based server with authentication and encryption is given in
`samples/file_based_server`. It uses AES to encrypt the message body.

A respective client can include the `lcxx::client` CMake target to enable the HTTP client libraries. A simple example to query a resource would look like this:

.. code-block:: c++
    using namespace lcxx;

    auto main() -> int
    {
        net::string_request req;
        req.base().insert( "key", "value" );
        req.body() = "Whatever string body you would need";
        req.set( net::http::field::content_type, "text/html" );

        auto response = client::request( "localhost:8080/" + lic_endpoint, req, [&]( net::dynamic_response const & resp ) {

        if ( response.result() == net::http::status::ok ) {
            // my code on successful request
        } else {
            // my code on unsuccessful request
        }

        return 0;
    }

A more sophisticated sample which works in combination with the file based server example can be found in `samples/client`.

The required RSA key and SSL certificate can be generated with the following commands:
.. code-block::
    openssl genrsa -out key.pem 2048
    openssl req -x509 -key key.pem -out certificate.pem -sha256
