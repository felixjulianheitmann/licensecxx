#include <iostream>

#include <lcxx/identifiers/hardware.hpp>
#include <lcxx/identifiers/os.hpp>
#include <lcxx/lcxx.hpp>

// Can also be loaded from file
constexpr auto private_key = R"(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEArT6TBfYmVOrE38Y1FIwzW78Xkp8PQWCfJyjAjPWQafXznTuG
1JEoT0eMlDC1HwXTJwohldzirePphYxp6I4ezkoS3i+s9fMmbTzLgy3MceZa+aZk
CZi8adQxVV2mZPn+LxNU8aZVDLh67lcs8piSYDjdIIr+LvbGFlB+95BhsEwq35sD
ap6pYvo+9WlxcGxbDRZLViFlUIEgAOLmIqCrlxGSGOV4cz7T59tHCXOS2SXSechm
yONB8fMAVFBYUhBlJT80syZVb/SqD+7EIGPa9D45xGNCUXttwhT2HaZmEpCfjEuu
1VQRaWZZnY+jpMQ1XYTQQifoapj3WTGJo4MajQIDAQABAoIBADaf3Eus6TktLRZB
sMrhye/LuOsKLA05IfV+6/eGFhrS6hVRh5nOix80OCchlHpLo+BTWXOfSfn6Os9Z
Dqu85Am3YWudLLQADsWyvZiHAEeJknXU+QXtmrrjpQp9lNGR4YuGVOo8v+Ra5qfv
5MQ5Ry3/v/la2fqE5MB0cRQt+h9T3/8P0dJm9COJGF3msX4ALzj4l2+GOxpfqTBF
BlrlOc/ndEb2uUT7t4xesFeJi140E0Gl0IiFXNGd5jmk1jTGpvypkYsfqLqnxuEE
2i5CGSof2rvKS9TQvHu2ayw/HquBSzqyxYJUQl+/7BZneVl+WFwSeqfDfbzSZRX8
8B5u6QECgYEA1boG7DFCb2sVOmh0Xu7fPYwAOBaYsJpv56i/evVuUSevilB+RK/+
Gy+lvoyCsx8jG/tMOG0eWtNADV/O2FE6/jZTNgZj0Th08xMk/+2cUryxdgHsLflj
bCX6CIv/mbo5H2TFPzeYnfB9wEnKUIo/W1CLi/GzuloF8wLjJ3mASxUCgYEAz4K+
Zkk1S2F7BXR9KECDu/dThKDKFe4hCJMIFNplVhwdfm/DEulZJT5TgUZoQ9SUMAfW
cWCNbzXCBAHyYz3TLNBjpG1Ql4O1NwbzQPBSsGc1fPNDjBRYaVKHqj2UB8c+7+aI
toPxHDTKuCvJGAW1sJfKgL5KSZ0CyfT0nRewD5kCgYAIDG1eT6yUzY+LF4vqV0yI
4NDRS+iMHgTA0JYFZ2C0Ja5yov1eUNJc67puJpR1cmK8FwaTyWgvO21aE5WSh9yU
3i7cBfmUU2/0B3CJQsV0SC7WptPiF1YrKHL2B2+ktmKYUA8thkZ1DC1wJFc+GTax
laLrrjp6dhFrSVyMjALM0QKBgQDAr6ALLNl/CKKKWzPIh7eLd8qmsgNUv80OwDDV
5EIK5oqAmFjkm6e6jJhPx0gUDDYaL3zxxH0wkhN5UzF287a+uzZ7PUKDnrpLwXlp
iH7P7NZfEyhaz52VFxyAeTOW0W3gqAm0qOnfjinbQFU3qD2hICHa0Ff86P3o+DuY
D6HdqQKBgC+/j9dvFMoB8oE3zpA7D20Wh3qSFUf7cBD3FmBg89+qIiYvitGnTY0k
CuOsl7rQhL0KAr4yfYigxgeBQXEowRVTLdhXNQBb5ux5qX0T9PpJHkoIVpwGspf+
7BNtOAIpetC5vz4UTaNWiDxFiqg0zlT7+YCVyDMNPHyuAas687gP
-----END RSA PRIVATE KEY-----
)";

auto main() -> int
{
    lcxx::license license;

    // Push optional data into the license file
    license.push_content( "some key", "some value" );

    license.push_content( "user", "SomeUsername" );
    license.push_content( "email", "SomeMail@SomeProvider.org" );

    auto key = lcxx::crypto::load_key( std::string{ private_key }, lcxx::crypto::key_type::private_key );
    std::cout << lcxx::to_json( license, key ).dump( 4 ) << std::endl;
}