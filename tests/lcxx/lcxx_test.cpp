#include <gtest/gtest.h>

#include <string>

#include <fixtures.hpp>
#include <lcxx/lcxx.hpp>
#include <lcxx/rsa.hpp>

auto license_factory() -> lcxx::license
{

    lcxx::license lic;
    lic.push_content( "some  key", "some value" );
    lic.push_content( "1", "1" );
    lic.push_content( "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345"
                      "678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901"
                      "234567890",
                      "" );
    return lic;
}

TEST_F( rsa_key_fixture, License_Json_Round_Trip_Verify )
{
    auto lic = license_factory();

    auto [lic_restored, signature] = lcxx::from_json( lcxx::to_json( lic, private_key ) );

    EXPECT_TRUE( lcxx::verify_license( lic_restored, signature, public_key ) );
}

TEST_F( rsa_key_fixture, License_Round_Trip_Verify )
{
    auto const lic       = license_factory();
    auto const signature = lcxx::sign( lic, private_key );

    EXPECT_TRUE( verify_license( lic, signature, public_key ) );
}