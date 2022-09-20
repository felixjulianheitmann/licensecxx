#ifndef LCXX__LCXX_VERIFIER_HPP__
#define LCXX__LCXX_VERIFIER_HPP__

#include <lcxx/crypto.hpp>
#include <lcxx/license.hpp>

namespace lcxx {

    /**
     * @brief sign the content of a license object with a private RSA key and return the signature
     *
     * @param lic the license object to sign
     * @param private_key the private key used to generate the signature
     * @return std::vector< std::byte > the binary signature in form of a byte vector
     */
    auto sign( license const & lic, crypto::rsa_key_t const private_key ) -> std::vector< std::byte >;

    /**
     * @brief check the contents of a license object against a signature by using a public RSA key for decryption
     *
     * @param lic the license object to check against the signature
     * @param signature the signature of the original license object
     * @param public_key the public key which must correspond to the private key used for encryption
     * @return true if the license and public key match the original license and private key used for the signature
     * @return false otherwise
     */
    auto verify_license( license const & lic, std::vector< std::byte > const & signature,
                         crypto::rsa_key_t const public_key ) -> bool;

}  // namespace lcxx

#endif  // LCXX__LCXX_VERIFIER_HPP__
