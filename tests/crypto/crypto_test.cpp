#include <gtest/gtest.h>

#include <string>

#include <fixtures.hpp>
#include <lcxx/crypto.hpp>

#ifndef TEST_RESOURCE_PATH
#define TEST_RESOURCE_PATH "."
#endif

TEST_F( key_fixture, Load_Key_String_Test )
{
    using path = std::filesystem::path;
    EXPECT_TRUE( lcxx::crypto::load_key( path( TEST_RESOURCE_PATH ) / path( "private_key.rsa" ),
                                         lcxx::crypto::key_type::private_key ) );
    EXPECT_TRUE( lcxx::crypto::load_key( path( TEST_RESOURCE_PATH ) / path( "public_key" ),
                                         lcxx::crypto::key_type::public_key ) );
}

TEST_F( key_fixture, Load_Key_File_Test )
{
    EXPECT_TRUE( private_key );
    EXPECT_TRUE( public_key );
}

TEST_F( key_fixture, Sign_Round_Trip_Test )
{

    std::string const test_string = "my arbitrary test string";
    auto const        signature   = lcxx::crypto::sign( test_string, private_key );
    auto const        verified    = lcxx::crypto::verify_signature( test_string, signature, public_key );

    EXPECT_TRUE( verified );
}

TEST_F( key_fixture, Sign_Round_Trip_Error_Test )
{
    std::string const test_string = "my arbitrary test string";
    auto const        signature   = lcxx::crypto::sign( test_string, private_key );
    auto const verified = lcxx::crypto::verify_signature( std::string( "_" ) + test_string, signature, public_key );

    EXPECT_FALSE( verified );
}

std::string private_key_str = R"(
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

std::string public_key_str = R"(
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
