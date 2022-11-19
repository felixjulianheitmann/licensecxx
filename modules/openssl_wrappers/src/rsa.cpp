#include <lcxx/rsa.hpp>

#include <fstream>
#include <vector>

#include <openssl/bio.h>
#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include <lcxx/hash.hpp>

namespace lcxx::crypto::rsa {

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
    }

    auto load_key( std::filesystem::path const & key_path, key_type const type ) -> key_t
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

    auto load_key( std::string const & key, key_type const type ) -> key_t
    {
        std::unique_ptr< BIO, decltype( &bio_deleter ) > bio(
            BIO_new_mem_buf( static_cast< const void * >( key.c_str() ), key.size() ), bio_deleter );

        key_t rsa_key = { nullptr, rsa_deleter };

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

    auto key_to_bytes( key_t const key ) -> std::vector< std::byte >
    {
        BIGNUM const *n = nullptr, *e = nullptr, *d = nullptr;
        RSA_get0_key( key.get(), &n, &e, &d );

        auto n_bytes   = n ? BN_num_bytes( n ) : 0;
        auto e_bytes   = e ? BN_num_bytes( e ) : 0;
        auto d_bytes   = d ? BN_num_bytes( d ) : 0;
        auto max_bytes = std::max( std::max( n_bytes, e_bytes ), d_bytes );

        std::vector< std::byte > serialized( max_bytes * 3 );
        auto                     offset0 = 0 * max_bytes;
        auto                     offset1 = 1 * max_bytes;
        auto                     offset2 = 2 * max_bytes;
        if ( n )
            BN_bn2binpad( n, reinterpret_cast< unsigned char * >( serialized.data() + offset0 ), max_bytes );
        if ( e )
            BN_bn2binpad( e, reinterpret_cast< unsigned char * >( serialized.data() + offset1 ), max_bytes );
        if ( d )
            BN_bn2binpad( d, reinterpret_cast< unsigned char * >( serialized.data() + offset2 ), max_bytes );

        return serialized;
    }

    auto key_from_bytes( std::span< std::byte const > const bytes ) -> key_t
    {
        auto key     = key_t{ RSA_new(), rsa_deleter };
        auto offset0 = 0 * bytes.size() / 3;
        auto offset1 = 1 * bytes.size() / 3;
        auto offset2 = 2 * bytes.size() / 3;
        auto len     = bytes.size() / 3;
        auto n       = BN_bin2bn( reinterpret_cast< unsigned char const * >( bytes.data() + offset0 ), len, nullptr );
        auto e       = BN_bin2bn( reinterpret_cast< unsigned char const * >( bytes.data() + offset1 ), len, nullptr );
        auto d       = BN_bin2bn( reinterpret_cast< unsigned char const * >( bytes.data() + offset2 ), len, nullptr );
        RSA_set0_key( key.get(), n, e, d );

        return key;
    }

    auto sign( std::string const & input_string, key_t const private_key ) -> std::vector< std::byte >
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
                           key_t const public_key ) -> bool
    {
        auto hashed = hash::sha512( std::string{ reference } );

        return RSA_verify( encryption_algorithm, reinterpret_cast< unsigned char const * >( hashed.data() ),
                           hashed.size(), reinterpret_cast< unsigned char const * >( signature.data() ),
                           signature.size(), public_key.get() );
    }

    auto encrypt( std::span< std::byte const > const input, key_t key, key_type const type ) -> std::vector< std::byte >
    {
        auto encrypt = [&]( auto && func, int padding ) {
            std::vector< std::byte > encrypted( RSA_size( key.get() ), std::byte{} );
            auto                     error = []() {
                throw std::runtime_error( "RSA Encrypt error: input buffer longer than encryptable through key." );
            };
            switch ( padding ) {
            case RSA_PKCS1_OAEP_PADDING:
                if ( input.size() > encrypted.size() - 42 )
                    error();
                break;
            case RSA_PKCS1_PADDING:
                if ( input.size() > encrypted.size() - 11 )
                    error();
                break;
            }

            auto ret = func( input.size(), reinterpret_cast< unsigned char const * >( input.data() ),
                             reinterpret_cast< unsigned char * >( encrypted.data() ), key.get(), padding );
            if ( ret == -1 )
                throw std::runtime_error( "RSA encryption error" );

            return encrypted;
        };

        switch ( type ) {
        case key_type::private_key:
            return encrypt( RSA_private_encrypt, RSA_PKCS1_PADDING );
        case key_type::public_key:
            return encrypt( RSA_public_encrypt, RSA_PKCS1_OAEP_PADDING );
        }
        return {};
    }

    auto decrypt( std::span< std::byte const > const input, key_t key, key_type const type ) -> std::vector< std::byte >
    {
        auto decrypt = [&]( auto && func, int padding ) {
            std::vector< std::byte > decrypted;
            switch ( padding ) {
            case RSA_PKCS1_OAEP_PADDING:
                decrypted.resize( RSA_size( key.get() ) - 42 );
                break;
            case RSA_PKCS1_PADDING:
                decrypted.resize( RSA_size( key.get() ) - 11 );
                break;
            }

            auto ret = func( input.size(), reinterpret_cast< unsigned char const * >( input.data() ),
                             reinterpret_cast< unsigned char * >( decrypted.data() ), key.get(), padding );
            if ( ret == -1 )
                throw std::runtime_error( "RSA decryption error" );

            decrypted.resize( ret );

            return decrypted;
        };

        switch ( type ) {
        case key_type::private_key:
            return decrypt( RSA_private_decrypt, RSA_PKCS1_OAEP_PADDING );
        case key_type::public_key:
            return decrypt( RSA_public_decrypt, RSA_PKCS1_PADDING );
        }
        return {};
    }

}  // namespace lcxx::crypto::rsa
