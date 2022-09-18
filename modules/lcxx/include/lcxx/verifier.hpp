#ifndef LCXX__LCXX_VERIFIER_HPP__
#define LCXX__LCXX_VERIFIER_HPP__

#include <lcxx/crypto.hpp>
#include <lcxx/license.hpp>

namespace lcxx {

    auto sign( license const & lic, crypto::rsa_key_t const private_key ) -> std::vector< std::byte >;

    auto verify_license( license const & lic, std::vector< std::byte > const & signature,
                         crypto::rsa_key_t const public_key ) -> bool;

}  // namespace lcxx

#endif  // LCXX__LCXX_VERIFIER_HPP__
