#ifndef LCXX__CRYPTO_HASH_HPP__
#define LCXX__CRYPTO_HASH_HPP__

#include <span>
#include <string>

#include <openssl/md5.h>
#include <openssl/sha.h>

namespace lcxx::hash {

    using md5_hash_t    = std::array< std::byte, MD5_DIGEST_LENGTH >;
    using sha512_hash_t = std::array< std::byte, SHA512_DIGEST_LENGTH >;

    auto md5( std::string const & input ) -> md5_hash_t;
    auto md5( std::span< const std::byte > const input ) -> md5_hash_t;
    template < typename T >
    requires( sizeof( T ) == 1 ) auto md5( std::span< T > const input ) -> md5_hash_t
    {
        return md5(
            std::span< const std::byte >{ reinterpret_cast< std::byte const * >( input.data() ), input.size() } );
    }

    auto sha512( std::string const & input ) -> sha512_hash_t;
    auto sha512( std::span< const std::byte > const input ) -> sha512_hash_t;
    template < typename T >
    requires( sizeof( T ) == 1 ) auto sha512( std::span< T > const input ) -> sha512_hash_t
    {
        return sha512(
            std::span< const std::byte >{ reinterpret_cast< std::byte const * >( input.data() ), input.size() } );
    }

}  // namespace lcxx::hash

#endif  // LCXX__CRYPTO_HASH_HPP__
