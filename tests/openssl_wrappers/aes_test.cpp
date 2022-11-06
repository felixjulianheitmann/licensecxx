#include <gtest/gtest.h>

#include <algorithm>
#include <string>

#include <fixtures.hpp>
#include <keys.hpp>
#include <lcxx/aes.hpp>
#include <lcxx/rsa.hpp>

using namespace lcxx::crypto;

TEST_F( aes_key_fixture, Gen_Key_Test ) { EXPECT_TRUE( key ); }

TEST_F( aes_key_fixture, Encrypt_Round_Trip_Test )
{

    auto const test_string = std::string{ "my arbitrary test string" };
    auto const encrypted   = aes::encrypt( test_string, key );
    auto const decrypted   = aes::decrypt( encrypted, key );

    std::vector< std::byte > test_bytes;
    for ( auto const v : test_string )
        test_bytes.push_back( std::byte( v ) );

    EXPECT_NE( encrypted, decrypted );
    EXPECT_EQ( test_bytes, decrypted );
}

TEST_F( aes_key_fixture, Encrypt_AES_Key_With_RSA_Public_Enc_Test )
{
    auto rsa_pub_key   = rsa::load_key( std::string{ lcxx::tests::keys::public_key_str }, rsa::key_type::public_key );
    auto rsa_priv_key  = rsa::load_key( std::string{ lcxx::tests::keys::private_key_str }, rsa::key_type::private_key );
    auto aes_key_bytes = aes::key_to_bytes( key );

    auto const encrypted = rsa::encrypt( aes_key_bytes, rsa_pub_key, rsa::key_type::public_key );
    auto const decrypted = rsa::decrypt( encrypted, rsa_priv_key, rsa::key_type::private_key );

    auto aes_key = aes::key_from_bytes( decrypted );

    EXPECT_NE( encrypted, decrypted );
    EXPECT_EQ( aes_key_bytes, decrypted );
}

TEST_F( aes_key_fixture, Encrypt_AES_Key_With_RSA_Privaet_Enc_Test )
{
    auto rsa_pub_key   = rsa::load_key( std::string{ lcxx::tests::keys::public_key_str }, rsa::key_type::public_key );
    auto rsa_priv_key  = rsa::load_key( std::string{ lcxx::tests::keys::private_key_str }, rsa::key_type::private_key );
    auto aes_key_bytes = aes::key_to_bytes( key );

    auto const encrypted = rsa::encrypt( aes_key_bytes, rsa_priv_key, rsa::key_type::private_key );
    auto const decrypted = rsa::decrypt( encrypted, rsa_pub_key, rsa::key_type::public_key );

    EXPECT_NE( encrypted, decrypted );
    EXPECT_EQ( aes_key_bytes, decrypted );
}

TEST_F( aes_key_fixture, Encrypt_AES_Key_With_RSA_Privaet_Enc_Test_pub_enc )
{
    auto rsa_pub_key   = rsa::load_key( std::string{ lcxx::tests::keys::public_key_str }, rsa::key_type::public_key );
    auto rsa_priv_key  = rsa::load_key( std::string{ lcxx::tests::keys::private_key_str }, rsa::key_type::private_key );
    auto aes_key_bytes = aes::key_to_bytes( key );

    auto const encrypted = rsa::encrypt( aes_key_bytes, rsa_pub_key, rsa::key_type::public_key );
    auto const decrypted = rsa::decrypt( encrypted, rsa_priv_key, rsa::key_type::private_key );

    EXPECT_NE( encrypted, decrypted );
    EXPECT_EQ( aes_key_bytes, decrypted );
}
