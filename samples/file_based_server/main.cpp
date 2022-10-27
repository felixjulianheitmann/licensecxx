#include <filesystem>
#include <fstream>
#include <iostream>

#include <lcxx/aes.hpp>
#include <lcxx/server.hpp>

using namespace lcxx;

void on_get_license( net::dynamic_request const & req, net::string_response & resp, std::string const & path,
                     std::string const & body );
void on_post_license( net::dynamic_request const & req, net::string_response & resp, std::string const & path,
                      std::string const & body, crypto::rsa::key_t const sign_key );
void on_delete_license( net::dynamic_request const & req, net::string_response & resp, std::string const & path,
                        std::string const & body );

/*
 * The demo below shows how to setup a simple network server, that has the following features:
 *   - requests licenses (on GET)
 *   - perform license signing and saving (on POST)
 *   - delete licenses (on DELETE)
 * All queries are encrypted via AES256. Only clients that have the key can communicate with this server.
 * All license files are saved in a local 'licenses' folder.
 *
 * To enable SSL/TLS support ... TODO ;)
 */

using namespace lcxx::crypto;

auto main() -> int
{

    server server( "0.0.0.0", 8080, "certificate.pem", "key.pem" );
    auto   auth_key = rsa::load_key( std::filesystem::current_path() / std::filesystem::path( "private_key.rsa" ),
                                     rsa::key_type::private_key );
    auto   sign_key =
        rsa::load_key( std::filesystem::current_path() / "signature_priv_key.rsa", rsa::key_type::private_key );

    server.on_default( [&]( net::dynamic_request const & req ) {
        net::string_response resp;
        resp.result( net::http::status::bad_request );
        resp.set( net::http::field::content_type, "text/html" );
        resp.body() = "These are not the pages you are looking for.";
        return resp;
    } );

    // Setup endpoint handler for all requests to '/licenses' and sub-targets
    // Takes the 'key' header value as RSA-encrypted AES key
    // Decrypts the message body using that AES key and proceeds to execute the GET/POST/DELETE handlers
    server.on_endpoint( "/licenses/*", [&]( net::dynamic_request const & req ) {
        net::string_response resp;

        aes::key_t aes_key;
        if ( req.base().find( "key" ) == req.base().end() ) {
            resp.set( net::http::field::content_type, "text/html" );
            resp.body() = "Message did not contain valid key in 'key' field in header";
        }
        else {
            aes_key = aes::key_from_bytes( rsa::decrypt( req.base().at( "key" ), auth_key ) );
        }

        auto        encrypted_body = boost::beast::buffers_to_string( req.body().data() );
        std::string body;
        if ( !encrypted_body.empty() )
            body = aes::decrypt< std::string, std::string >( encrypted_body, aes_key );

        resp.set( net::http::field::content_type, "application/json" );
        std::string path = { req.target().begin(), req.target().end() };

        try {
            switch ( req.method() ) {
            case net::verb::get:
                on_get_license( req, resp, path, body );
                break;
            case net::verb::post:
                on_post_license( req, resp, path, body, sign_key );
                break;
            case net::verb::delete_:
                on_delete_license( req, resp, path, body );
                break;
            default:
                resp.body() = "Given verb not supported";
                break;
            }
        }
        catch ( std::exception & e ) {
            resp.set( net::http::field::content_type, "text/html" );
            resp.body() = std::string{ "Could not load license, error: " } + e.what();
        }

        return resp;
    } );

    try {
        server.run( server::run_option::sync );
    }
    catch ( std::exception & e ) {
        server.stop();
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

// Returns the requested file under /licenses/... if the decrypted message body matches the target path
void on_get_license( net::dynamic_request const & req, net::string_response & resp, std::string const & path,
                     std::string const & body )
{
    if ( body == path ) {
        if ( !std::filesystem::exists( path ) )
            throw std::runtime_error( "File not found" );
        std::ifstream ifs{ path };
        auto license = std::string{ std::istreambuf_iterator< char >{ ifs }, std::istreambuf_iterator< char >{} };
        resp.body()  = license;
    }
    else {
        throw std::runtime_error( "Decrypted body did not match path" );
    }
}

// Saves the decrypted license in message body to the desired target path
void on_post_license( net::dynamic_request const & req, net::string_response & resp, std::string const & path,
                      std::string const & body, rsa::key_t const sign_key )
{
    auto [lic, _] = lcxx::from_string( boost::beast::buffers_to_string( req.body().data() ) );
    lcxx::to_file( lic, path, sign_key );
    resp.body() = "Successfully saved\n";
}

// Deletes the requested file under /licenses/... if the decrypted message body matches the target path
void on_delete_license( net::dynamic_request const & req, net::string_response & resp, std::string const & path,
                        std::string const & body )
{
    if ( body == path ) {
        if ( !std::filesystem::exists( path ) )
            throw std::runtime_error( "File not found" );
        std::filesystem::remove( path );
        resp.body() = "Successfully deleted\n";
    }
    else {
        throw std::runtime_error( "Decrypted body did not match path" );
    }
}
