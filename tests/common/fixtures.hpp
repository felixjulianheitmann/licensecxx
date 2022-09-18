#ifndef LCXX__TESTS_COMMON_FIXTURES_HPP__
#define LCXX__TESTS_COMMON_FIXTURES_HPP__

#include <gtest/gtest.h>

#include <keys.hpp>
#include <lcxx/crypto.hpp>

class key_fixture : public ::testing::Test {
protected:
    lcxx::crypto::rsa_key_t private_key;
    lcxx::crypto::rsa_key_t public_key;

    key_fixture() :
        private_key( lcxx::crypto::load_key( std::string{ lcxx::tests::keys::private_key_str },
                                             lcxx::crypto::key_type::private_key ) ),
        public_key( lcxx::crypto::load_key( std::string{ lcxx::tests::keys::public_key_str },
                                            lcxx::crypto::key_type::public_key ) )
    {
    }
};

#endif  // LCXX__TESTS_COMMON_FIXTURES_HPP__
