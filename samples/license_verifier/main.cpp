#include <iostream>

#include <lcxx/crypto.hpp>
#include <lcxx/lcxx.hpp>

// Can both be loaded from file or elsewhere
constexpr auto lic_str = R"(
{
    "content": {
        "hardware": "",
        "some key": "some value",
        "user": ""
    },
    "signature": "VeWLaYWQpmODFvPLIZIf6AFH5Cu2au2UBpMVOhE6vT2YdKpbjDGI1mq/9I98BsMcu6EQg0zAYTlIg+7/dffx90WiI8JDk518YKqoraWdlyGVvm8Q8N9H+yDtGF/7zvPhzu7s17x4RWKYWYJ5r4e6iPAZLBmns9EcznhSfQfSDcgUJvx8yYMjNiFzPt/AXHeUG7D5/3g8WVR3Ky7yrDy0+0cokMVDJuD9hnyoYVXcXEro10zIamjgq4tVkVVwcrYJw98kOEZfxjF2y94gfGzj70CkuMd9aJpiFY5EtgiPDOePzHqf9kDvYwOxauDCt9U8N1qMrjxE0HZ5FBwtZpw/Rg=="
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

    auto key                  = lcxx::crypto::load_key( std::string{ public_key }, lcxx::crypto::key_type::public_key );
    auto [license, signature] = lcxx::from_json( std::string{ lic_str } );

    if ( !lcxx::verify_license( license, signature, key ) ) {
        std::cout << "This program is not licensed!" << std::endl;
        return -1;
    }

    return 0;
}