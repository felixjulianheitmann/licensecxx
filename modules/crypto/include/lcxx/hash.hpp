#ifndef LCXX__CRYPTO_HASH_HPP__
#define LCXX__CRYPTO_HASH_HPP__

#include <span>
#include <string>

#include <openssl/evp.h>

namespace lcxx::hash {

    enum class error {
        ok,
        ctx_allocation_failure,
        digest_init_failure,
        digest_update_failure,
        digest_finalization_failure,
    };

    using sha512_hash_t = std::array< std::byte, 64 >;

    /**
     * @brief calculates a SHA512 hashsum over an input string
     *
     * @param input the string that will be digested
     * @return sha512_hash_t the SHA512 hash in byte-array form
     */
    auto sha512( std::string const & input ) -> std::pair< sha512_hash_t, error >;
    auto sha512( std::span< const std::byte > const input ) -> std::pair< sha512_hash_t, error >;
    template < typename T >
        requires( sizeof( T ) == 1 )
    auto sha512( std::span< T > const input ) -> std::pair< sha512_hash_t, error >
    {
        return sha512(
            std::span< const std::byte >{ reinterpret_cast< std::byte const * >( input.data() ), input.size() } );
    }

}  // namespace lcxx::hash

#endif  // LCXX__CRYPTO_HASH_HPP__
