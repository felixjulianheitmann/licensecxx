#ifndef LCXX__CRYPTO_CRYPTO_HPP__
#define LCXX__CRYPTO_CRYPTO_HPP__

#include <cstddef>
#include <filesystem>
#include <string>

#include <openssl/rsa.h>

namespace lcxx::crypto {

    using rsa_key_t = std::shared_ptr< RSA >;

    enum class key_type {
        public_key,
        private_key,
    };

    /**
     * @brief loads a key (public/private) from string into memory.
     *
     * @param key the key data
     * @param type whether this is a public or private key
     * @param b64 if the string is base64 encoded or binary
     */
    auto load_key( std::string const & key, key_type type, bool with_header = true, bool b64 = true ) -> rsa_key_t;

    /**
     * @brief loads a key (public/private) from file into memory. File must be PEM type
     *
     * @param key_path file path to the corresponding key file
     * @param type whether this is a public or private key
     */
    auto load_key( std::filesystem::path const & key_path, key_type type ) -> rsa_key_t;

    /**
     * @brief if a private key has been loaded, this method will sign an input string and return the encrypted data
     *
     * @param string_to_sign the string that should be signed
     * @param base64 whether to encode the enrypted string as base64
     * @return std::string the encrypted string
     */
    auto sign( std::string const & input_string, rsa_key_t const private_key ) -> std::string;

    /**
     * @brief if a public key has been loaded, takes a reference string and checks if the signature matches that
     * string
     *
     * @param reference the input data that was used to create the signature
     * @param signature the signature that is checked against the reference data
     * @param base64 whether the signature is base64 encoded
     * @return true if the signature matches the reference string, given the loaded public key
     * @return false if signature and reference string do not match, given the loaded public key
     */
    auto verify_signature( std::string_view const reference, std::string_view const signature,
                           rsa_key_t const public_key ) -> bool;

}  // namespace lcxx::crypto

#endif  //  LCXX__CRYPTO_CRYPTO_HPP__