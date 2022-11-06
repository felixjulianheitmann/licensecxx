#ifndef LCXX__CRYPTO_RSA_HPP__
#define LCXX__CRYPTO_RSA_HPP__

#include <cstddef>
#include <filesystem>
#include <span>
#include <string>
#include <vector>

#include <openssl/rsa.h>

namespace lcxx::crypto::rsa {

    using key_t = std::shared_ptr< RSA >;

    enum class key_type {
        public_key,
        private_key,
    };

    /**
     * @brief loads a key (public/private) from string into memory.
     *
     * @param key the RSA key data in PEM format
     * @param type whether this is a public or private key
     */
    auto load_key( std::string const & key, key_type type ) -> key_t;

    /**
     * @brief loads a key (public/private) from file into memory. File must be PEM type
     *
     * @param key_path file path to the corresponding RSA key file in PEM format
     * @param type whether this is a public or private key
     */
    auto load_key( std::filesystem::path const & key_path, key_type type ) -> key_t;

    auto key_to_bytes( key_t const key ) -> std::vector< std::byte >;
    auto key_from_bytes( std::span< std::byte const > const bytes ) -> key_t;
    template < std::ranges::contiguous_range R >
    requires( sizeof( std::ranges::range_value_t< std::remove_cvref_t< R > > ) ==
              1 ) auto key_from_bytes( R const & bytes, key_t const key ) -> key_t
    {
        return key_from_bytes( std::span< std::byte const >(
            reinterpret_cast< std::byte const * >( std::ranges::data( bytes ) ), std::ranges::size( bytes ) ) );
    }

    /**
     * @brief this method will sign an input string and a private RSA key and return the encrypted data
     *
     * @param input_string the string that should be signed
     * @param private_key the private RSA key used for the signature
     * @return std::string the encrypted string
     */
    auto sign( std::string const & input_string, key_t const private_key ) -> std::vector< std::byte >;

    /**
     * @brief takes a reference string, a corresponding signature and a public key to verify if the signature matches
     * that reference string
     *
     * @param reference the input data that was used to create the signature
     * @param signature the signature that is checked against the reference data
     * @param public_key the public RSA key corresponding to the one used for the signature
     * @return true if the signature matches the reference string, given the loaded public key
     * @return false if signature and reference string do not match, given the loaded public key
     */
    auto verify_signature( std::string_view const reference, std::vector< std::byte > const & signature,
                           key_t const public_key ) -> bool;

    auto encrypt( std::span< std::byte const > const input, key_t key, key_type const type )
        -> std::vector< std::byte >;

    template < std::ranges::contiguous_range R >
    requires( sizeof( std::ranges::range_value_t< std::remove_cvref_t< R > > ) ==
              1 ) auto encrypt( R const & input, key_t key, key_type const type ) -> std::vector< std::byte >
    {
        return encrypt(
            std::span< std::byte const >( reinterpret_cast< std::byte const * >( std::ranges::data( input ) ),
                                          std::ranges::size( input ) ),
            key, type );
    }

    auto decrypt( std::span< std::byte const > const input, key_t key, key_type const type )
        -> std::vector< std::byte >;

    template < std::ranges::contiguous_range R >
    requires( sizeof( std::ranges::range_value_t< std::remove_cvref_t< R > > ) ==
              1 ) auto decrypt( R const & input, key_t key, key_type const type ) -> std::vector< std::byte >
    {
        return decrypt(
            std::span< std::byte const >( reinterpret_cast< std::byte const * >( std::ranges::data( input ) ),
                                          std::ranges::size( input ) ),
            key, type );
    }

}  // namespace lcxx::crypto::rsa

#endif  //  LCXX__CRYPTO_RSA_HPP__