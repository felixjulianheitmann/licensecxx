#include <lcxx/verifier.hpp>

#include <algorithm>

namespace lcxx {

    auto sign( license const & lic, crypto::rsa_key_t const private_key ) -> std::vector< std::byte >
    {
        return crypto::sign( lic.stringify(), private_key );
    }

    auto verify_license( license const & lic, std::vector< std::byte > const & signature,
                         crypto::rsa_key_t const public_key ) -> bool
    {
        return crypto::verify_signature( lic.stringify(), signature, public_key );
    }

}  // namespace lcxx
