#include <lcxx/crypto.hpp>

#include <fstream>
#include <vector>

#include <fmt/core.h>
#include <openssl/bio.h>
#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include <lcxx/hash.hpp>

namespace lcxx::crypto {

    constexpr auto encryption_algorithm = NID_sha512WithRSAEncryption;

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

    auto load_key( std::filesystem::path const & key_path, key_type const type ) -> rsa_key_t
    {
        std::ifstream ifs{ key_path.string() };
        if ( ifs ) {
            std::string key{ std::istreambuf_iterator< char >{ ifs }, std::istreambuf_iterator< char >{} };
            return load_key( key, type );
        }
        else {
            throw std::runtime_error( std::string{ "Could not open key file at: " } + key_path.string() );
        }
    }

    auto load_key( std::string const & key, key_type const type ) -> rsa_key_t
    {
        // TODO USE SCOPED UNIQUE PTR
        std::unique_ptr< BIO, decltype( &bio_deleter ) > bio(
            BIO_new_mem_buf( static_cast< const void * >( key.c_str() ), key.size() ), bio_deleter );

        rsa_key_t rsa_key = { nullptr, rsa_deleter };

        if ( type == key_type::private_key ) {
            rsa_key = { PEM_read_bio_RSAPrivateKey( bio.get(), nullptr, nullptr, nullptr ), rsa_deleter };
        }
        else {
            rsa_key = { PEM_read_bio_RSA_PUBKEY( bio.get(), nullptr, nullptr, nullptr ), rsa_deleter };
        }

        if ( !rsa_key )
            throw std::logic_error( "Could not properly parse the provided key" );
        return rsa_key;
    }

    auto sign( std::string const & input_string, rsa_key_t const private_key ) -> std::vector< std::byte >
    {

        auto hashed = hash::sha512( input_string );

        std::vector< std::byte > signature( RSA_size( private_key.get() ) );
        unsigned int             n_signed_bytes = 0;
        if ( !RSA_sign( encryption_algorithm, reinterpret_cast< unsigned char const * >( hashed.data() ), hashed.size(),
                        reinterpret_cast< unsigned char * >( signature.data() ), &n_signed_bytes,
                        private_key.get() ) ) {
            throw std::runtime_error( "Could not sign input data." );
        }

        return signature;
    }

    auto verify_signature( std::string_view const reference, std::vector< std::byte > const & signature,
                           rsa_key_t const public_key ) -> bool
    {
        auto hashed = hash::sha512( std::string{ reference } );

        return RSA_verify( encryption_algorithm, reinterpret_cast< unsigned char const * >( hashed.data() ),
                           hashed.size(), reinterpret_cast< unsigned char const * >( signature.data() ),
                           signature.size(), public_key.get() );
    }

}  // namespace lcxx::crypto
