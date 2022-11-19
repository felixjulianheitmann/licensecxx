#ifndef LCXX__CRYPTO_AES_HPP__
#define LCXX__CRYPTO_AES_HPP__

#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <vector>

#include <openssl/aes.h>
#include <openssl/evp.h>

#include <lcxx/crypto_common.hpp>

namespace lcxx::crypto::aes {

    struct key_struct {
        using buf_type = std::array< unsigned char, 32 >;
        buf_type key;
        buf_type iv;
    };
    using key_t = std::shared_ptr< key_struct >;

    using salt = std::optional< uint64_t >;

    /**
     * @brief loads a key from file into memory. File must be PEM type
     *
     * @param key_path file path to the corresponding RSA key file in PEM format
     */
    auto gen_key( salt const salt, std::span< unsigned char const > const key_data, int n_rounds = 5 ) -> key_t;
    template < detail::contiguous_byte_range R, detail::contiguous_byte_range S >
    inline auto gen_key( R const & salt_bytes, S const & key_data, int n_rounds = 5 ) -> key_t
    {
        if ( std::ranges::size( salt_bytes ) < 8 )
            throw std::invalid_argument( "AES key generation: Salt must be at least 8 bytes long." );

        return gen_key( *reinterpret_cast< uint64_t const * >( std::ranges::data( salt_bytes ) ),
                        std::span< unsigned char const >(
                            reinterpret_cast< unsigned char const * >( std::ranges::data( key_data ) ),
                            std::ranges::size( key_data ) ),
                        n_rounds );
    }

    auto key_to_bytes( key_t const key ) -> std::vector< std::byte >;
    auto key_from_bytes( std::span< std::byte const > const bytes ) -> key_t;

    template < detail::contiguous_byte_range R > inline auto key_from_bytes( R const & bytes, key_t const key ) -> key_t
    {
        return key_from_bytes( std::span< std::byte const >(
            reinterpret_cast< std::byte const * >( std::ranges::data( bytes ) ), std::ranges::size( bytes ) ) );
    }

    /**
     * @brief this method will sign an input byte range and an AES key and return the encrypted data
     *
     * @param input the string that should be signed
     * @param key the AES key used for the encryption
     * @return std::string the encrypted string
     */
    auto encrypt( std::span< std::byte const > const input, key_t const key ) -> std::vector< std::byte >;

    template < detail::contiguous_byte_range R, detail::contiguous_byte_range S = std::vector< std::byte > >
    inline auto encrypt( R const & input, key_t const key ) -> S
    {
        auto encrypted =
            encrypt( std::span< std::byte const >( reinterpret_cast< std::byte const * >( std::ranges::data( input ) ),
                                                   std::ranges::size( input ) ),
                     key );
        S tmp{};
        std::ranges::transform( encrypted, std::back_inserter( tmp ), []( auto const v ) {
            return *reinterpret_cast< std::ranges::range_value_t< S > const * >( &v );
        } );
        return tmp;
    }

    /**
     * @brief takes a reference string, a corresponding signature and a public key to verify if the
     * signature matches that reference string
     *
     * @param reference the input data that was used to create the signature
     * @param public_key the public RSA key corresponding to the one used for the signature
     * @return true if the signature matches the reference string, given the loaded public key
     * @return false if signature and reference string do not match, given the loaded public key
     */
    auto decrypt( std::span< std::byte const > const input, key_t const key ) -> std::vector< std::byte >;

    template < detail::contiguous_byte_range R, detail::contiguous_byte_range S = std::vector< std::byte > >
    inline auto decrypt( R const & input, key_t const key ) -> S
    {
        auto decrypted =
            decrypt( std::span< std::byte const >{ reinterpret_cast< std::byte const * >( std::ranges::data( input ) ),
                                                   std::ranges::size( input ) },
                     key );
        S tmp{};
        std::ranges::transform( decrypted, std::back_inserter( tmp ), []( auto const v ) {
            return *reinterpret_cast< std::ranges::range_value_t< S > const * >( &v );
        } );
        return tmp;
    }

}  // namespace lcxx::crypto::aes

#endif  //  LCXX__CRYPTO_AES_HPP__