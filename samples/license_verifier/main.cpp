#include <iostream>

#include <lcxx/lcxx.hpp>

// Can both be loaded from file or elsewhere
constexpr auto lic_str = R"(
{
    "content": {
        "another key": "another value",
        "email": "john.doe@happymail.com",
        "some key": "some value"
    },
    "signature": "f4puZx7UnIHPM+LskRDBLzLHbgKaTuMHYRGl/F5xCInrh1hs36bPh8ONtvBvZS1HmCww8jX+NPGdzfVInmJn/C+1ptdirJ/FBPEx0HGbpNSmEMzVguL0DO8Kvb4zhO1SiiZVKBfE1rTGlcf06hfwDcw0krx+dXaWWYeXqhDeGD+m/2ihNs+r0YgLkRwFRAbDUxz2xRsrobdfB3D178a8i0W8ElwY0tfy38OF4woSsZUNpyiFreIHzGbqLs/P5L262/N6zs98H65ZWOF7KDLiMc7tMOIXptlMxk/SNTncrlCZ71Kzgq2/4hVRzNbWeu+thJA21YTSdOTYk8d5QoarRQ=="
}
)";

constexpr auto public_key = R"(
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEArT6TBfYmVOrE38Y1FIwz
W78Xkp8PQWCfJyjAjPWQafXznTuG1JEoT0eMlDC1HwXTJwohldzirePphYxp6I4e
zkoS3i+s9fMmbTzLgy3MceZa+aZkCZi8adQxVV2mZPn+LxNU8aZVDLh67lcs8piS
YDjdIIr+LvbGFlB+95BhsEwq35sDap6pYvo+9WlxcGxbDRZLViFlUIEgAOLmIqCr
lxGSGOV4cz7T59tHCXOS2SXSechmyONB8fMAVFBYUhBlJT80syZVb/SqD+7EIGPa
9D45xGNCUXttwhT2HaZmEpCfjEuu1VQRaWZZnY+jpMQ1XYTQQifoapj3WTGJo4Ma
jQIDAQAB
-----END PUBLIC KEY-----
)";

auto main() -> int
{

    auto key = lcxx::crypto::rsa::load_key( std::string{ public_key }, lcxx::crypto::rsa::key_type::public_key );
    auto [license, signature] = lcxx::from_json( std::string{ lic_str } );

    if ( !lcxx::verify_license( license, signature, key ) ) {
        std::cout << "This program is not licensed!" << std::endl;
        return -1;
    }

    return 0;
}