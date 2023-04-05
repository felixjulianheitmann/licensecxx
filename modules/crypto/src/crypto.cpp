#include <lcxx/crypto.hpp>

#include <array>
#include <cstddef>
#include <fstream>
#include <vector>

#include <openssl/bio.h>
#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include <lcxx/hash.hpp>

namespace lcxx::crypto {

    namespace {

        void context_deleter( EVP_MD_CTX * ctx )
        {
            if ( ctx )
                EVP_MD_CTX_destroy( ctx );
        }

        using context = std::unique_ptr< EVP_MD_CTX, decltype( &context_deleter ) >;

        auto create_context() -> context
        {
            context ctx( EVP_MD_CTX_create(), context_deleter );
            if ( !ctx ) {
                throw std::runtime_error( "Could not create OpenSSL context" );
            }

            return ctx;
        }
    }  // namespace

    void bio_deleter( BIO * bio )
    {
        if ( bio )
            BIO_free( bio );
    }

    void key_deleter( EVP_PKEY * key )
    {
        if ( key )
            EVP_PKEY_free( key );
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
        std::unique_ptr< BIO, decltype( &bio_deleter ) > bio(
            BIO_new_mem_buf( static_cast< const void * >( key.c_str() ), key.size() ), bio_deleter );

        rsa_key_t rsa_key = { nullptr, key_deleter };

        if ( type == key_type::private_key ) {
            rsa_key = { PEM_read_bio_PrivateKey( bio.get(), nullptr, nullptr, nullptr ), key_deleter };
        }
        else {
            rsa_key = { PEM_read_bio_PUBKEY( bio.get(), nullptr, nullptr, nullptr ), key_deleter };
        }

        if ( !rsa_key )
            throw std::logic_error( "Could not properly parse the provided key" );
        return rsa_key;
    }

    auto sign( std::string const & input_string, rsa_key_t const private_key ) -> std::vector< std::byte >
    {
        static_assert( sizeof( unsigned char ) == sizeof( std::byte ) );
        auto ctx = create_context();

        if ( 1 != EVP_DigestSignInit( ctx.get(), NULL, EVP_sha512(), NULL, private_key.get() ) ) {
            throw std::runtime_error( "Could not initialize openSSL digest" );
        }

        if ( 1 != EVP_DigestSignUpdate( ctx.get(), input_string.data(), input_string.size() ) ) {
            throw std::runtime_error( "Could not updat the openSSL digest" );
        }

        size_t len;
        if ( 1 != EVP_DigestSignFinal( ctx.get(), NULL, &len ) ) {
            throw std::runtime_error( "Could not calculate signature length" );
        }

        std::vector< std::byte > signature( len );
        if ( 1 != EVP_DigestSignFinal( ctx.get(), reinterpret_cast< unsigned char * >( signature.data() ), &len ) ) {
            throw std::runtime_error( "Could not calculate signature" );
        }

        return signature;
    }

    auto verify_signature( std::string_view const reference, std::vector< std::byte > const & signature,
                           rsa_key_t const public_key ) -> bool
    {

        auto ctx = create_context();
        if ( 1 != EVP_DigestVerifyInit( ctx.get(), NULL, EVP_sha512(), NULL, public_key.get() ) ) {
            throw std::runtime_error( "could not initialize openSSL digest" );
        }

        if ( 1 != EVP_DigestVerifyUpdate( ctx.get(), reinterpret_cast< void const * >( reference.data() ),
                                          reference.size() ) ) {
            throw std::runtime_error( "could not read in openSSL reference data" );
        }

        return ( 1 == EVP_DigestVerifyFinal( ctx.get(), reinterpret_cast< unsigned char const * >( signature.data() ),
                                             signature.size() ) );
    }

}  // namespace lcxx::crypto
