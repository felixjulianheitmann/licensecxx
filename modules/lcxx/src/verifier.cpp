#include <lcxx/verifier.hpp>

#include <algorithm>

namespace lcxx {

    auto verify_license( license const & lic, std::string const & signature, crypto::rsa_key_t const public_key )
        -> bool
    {
        return crypto::verify_signature( lic.stringify(), signature, public_key );
    }

}  // namespace lcxx
