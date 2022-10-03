#ifndef LCXX__TESTS_COMMON_FIXTURES_HPP__
#define LCXX__TESTS_COMMON_FIXTURES_HPP__

#include <gtest/gtest.h>

#include <keys.hpp>
#include <lcxx/aes.hpp>
#include <lcxx/rsa.hpp>

class rsa_key_fixture : public ::testing::Test {
protected:
    lcxx::crypto::rsa::key_t private_key;
    lcxx::crypto::rsa::key_t public_key;

    rsa_key_fixture() :
        private_key( lcxx::crypto::rsa::load_key( std::string{ lcxx::tests::keys::private_key_str },
                                                  lcxx::crypto::rsa::key_type::private_key ) ),
        public_key( lcxx::crypto::rsa::load_key( std::string{ lcxx::tests::keys::public_key_str },
                                                 lcxx::crypto::rsa::key_type::public_key ) )
    {
    }
};

class aes_key_fixture : public ::testing::Test {
protected:
    lcxx::crypto::aes::key_t key;

    aes_key_fixture() :
        key( lcxx::crypto::aes::gen_key( std::array< unsigned char, 8 >{ 1, 2, 3, 4, 5, 6, 7, 8 },
                                         generate_key_data() ) )
    {
    }

    std::array< unsigned char, 10 > generate_key_data() { return { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; }
};

#endif  // LCXX__TESTS_COMMON_FIXTURES_HPP__
