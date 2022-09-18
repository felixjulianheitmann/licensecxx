#include <lcxx/crypto.hpp>

#include <fstream>
#include <vector>

#include <fmt/core.h>
#include <openssl/bio.h>
#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

namespace lcxx::crypto {

    void bio_deleter( BIO * bio )
    {
        if ( bio )
            BIO_free( bio );
    }

    void rsa_deleter( RSA * rsa )
    {
        if ( rsa )
            RSA_free( rsa );
    };

    rsa_key_t load_key( std::filesystem::path const & key_path, key_type const type )
    {
        std::ifstream ifs{ key_path.string() };
        if ( ifs ) {
            std::string key{ std::istreambuf_iterator< char >{ ifs }, std::istreambuf_iterator< char >{} };
            return load_key( key, type, true, true );
        }
        else {
            throw std::runtime_error( fmt::format( "Could not open key file at: {}", key_path.string() ) );
        }
    }

    rsa_key_t load_key( std::string const & key, key_type const type, bool with_header, bool b64 )
    {
        // TODO USE SCOPED UNIQUE PTR
        std::unique_ptr< BIO, decltype( &bio_deleter ) > bio(
            BIO_new_mem_buf( static_cast< const void * >( key.c_str() ), key.size() ), bio_deleter );

        rsa_key_t rsa_key = { nullptr, rsa_deleter };

        if ( type == key_type::private_key ) {
            rsa_key = { PEM_read_bio_RSAPrivateKey( bio.get(), nullptr, nullptr, nullptr ), rsa_deleter };
        }
        else {
            rsa_key = { PEM_read_bio_RSAPublicKey( bio.get(), nullptr, nullptr, nullptr ), rsa_deleter };
        }

        if ( !rsa_key )
            throw std::logic_error( "Could not properly parse the provided key" );
        return rsa_key;
    }

    std::string sign( std::string const & input_string, rsa_key_t const private_key )
    {

        std::vector< unsigned char > input_bytes{ input_string.begin(), input_string.end() };
        std::vector< unsigned char > signed_bytes( RSA_size( private_key.get() ) );
        unsigned int                 n_signed_bytes = 0;
        if ( !RSA_sign( NID_sha256, input_bytes.data(), input_bytes.size(), signed_bytes.data(), &n_signed_bytes,
                        private_key.get() ) ) {
            throw std::runtime_error( "Could not sign input data." );
        }

        return std::string{ signed_bytes.begin(), signed_bytes.end() };
    }

    bool verify_signature( std::string_view const reference, std::string_view const signature,
                           rsa_key_t const public_key )
    {

        std::vector< unsigned char > input_bytes{ reference.begin(), reference.end() };
        std::vector< unsigned char > signature_bytes( signature.begin(), signature.end() );
        unsigned int                 n_signed_bytes = 0;
        return RSA_verify( NID_sha256, input_bytes.data(), input_bytes.size(), signature_bytes.data(),
                           signature_bytes.size(), public_key.get() );
    }

}  // namespace lcxx::crypto
