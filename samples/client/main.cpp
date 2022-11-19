#include <iostream>

#include <lcxx/aes.hpp>
#include <lcxx/client.hpp>
#include <lcxx/encoding.hpp>
#include <lcxx/lcxx.hpp>
#include <lcxx/rsa.hpp>

using namespace lcxx;
namespace rsa = crypto::rsa;
namespace aes = crypto::aes;
namespace fs  = std::filesystem;

auto main() -> int
{
    auto const auth_key     = rsa::load_key( fs::current_path() / "public_key", rsa::key_type::public_key );
    auto const verify_key   = rsa::load_key( fs::current_path() / "signature_pub_key", rsa::key_type::public_key );
    auto const lic_endpoint = std::string( "/licenses/mylicense.json" );

    auto aes_key = aes::gen_key( "8byteSal", "mykeydata" );
    auto encrypted_aes_key =
        encode::base64( rsa::encrypt( aes::key_to_bytes( aes_key ), auth_key, rsa::key_type::public_key ) );
    auto encrypted_body = encode::base64( aes::encrypt( lic_endpoint, aes_key ) );

    net::string_request req;
    req.method( net::http::verb::get );
    req.set( net::http::field::content_type, "text/html" );
    req.target( lic_endpoint );
    req.body() = encrypted_body;
    req.base().insert( "key", encrypted_aes_key );
    req.prepare_payload();

    bool verified = false;
    try {
        auto resp = client::request( req, "localhost", "443" );

        if ( resp && resp->result() == net::http::status::ok ) {
            auto const [license, signature] = from_string( boost::beast::buffers_to_string( resp->body().data() ) );
            if ( verify_license( license, signature, verify_key ) ) {
                verified = true;
            }
        }

        if ( verified ) {
            std::cout << "License verified!" << std::endl;
            return 0;
        }
        else {
            std::cout << "License verification failed!" << std::endl;
            return 1;
        }
    }
    catch ( std::exception & e ) {
        std::cout << "Error occurred: " << e.what() << std::endl;
    }
}