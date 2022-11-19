#include <gtest/gtest.h>

#include <array>

#include <lcxx/hash.hpp>

TEST( Crypto_Tests, MD5_Hash_String_Test )
{
    constexpr auto                        test_string    = "my arbitrary test string";
    constexpr std::array< std::byte, 16 > reference_hash = {
        std::byte{ 0xFD }, std::byte{ 0xAD }, std::byte{ 0xC7 }, std::byte{ 0xD1 },
        std::byte{ 0x83 }, std::byte{ 0x40 }, std::byte{ 0x95 }, std::byte{ 0xEB },
        std::byte{ 0xB9 }, std::byte{ 0x01 }, std::byte{ 0xC2 }, std::byte{ 0x6F },
        std::byte{ 0xA7 }, std::byte{ 0xB8 }, std::byte{ 0xF1 }, std::byte{ 0xED },
    };

    auto md5 = lcxx::hash::md5( test_string );
    EXPECT_EQ( md5, reference_hash );
}

TEST( Crypto_Tests, MD5_Hash_Byte_Test )
{
    std::vector< std::byte > const test_bytes = {
        std::byte{ 'm' }, std::byte{ 'y' }, std::byte{ ' ' }, std::byte{ 'a' }, std::byte{ 'r' }, std::byte{ 'b' },
        std::byte{ 'i' }, std::byte{ 't' }, std::byte{ 'r' }, std::byte{ 'a' }, std::byte{ 'r' }, std::byte{ 'y' },
        std::byte{ ' ' }, std::byte{ 't' }, std::byte{ 'e' }, std::byte{ 's' }, std::byte{ 't' }, std::byte{ ' ' },
        std::byte{ 's' }, std::byte{ 't' }, std::byte{ 'r' }, std::byte{ 'i' }, std::byte{ 'n' }, std::byte{ 'g' },
    };
    constexpr std::array< std::byte, 16 > reference_hash = {
        std::byte{ 0xFD }, std::byte{ 0xAD }, std::byte{ 0xC7 }, std::byte{ 0xD1 },
        std::byte{ 0x83 }, std::byte{ 0x40 }, std::byte{ 0x95 }, std::byte{ 0xEB },
        std::byte{ 0xB9 }, std::byte{ 0x01 }, std::byte{ 0xC2 }, std::byte{ 0x6F },
        std::byte{ 0xA7 }, std::byte{ 0xB8 }, std::byte{ 0xF1 }, std::byte{ 0xED },
    };

    auto md5 = lcxx::hash::md5( test_bytes );
    EXPECT_EQ( md5, reference_hash );
}

TEST( Crypto_Tests, MD5_Hash_Other_Type_Test )
{
    std::vector< uint8_t > const test_bytes = {
        'm', 'y', ' ', 'a', 'r', 'b', 'i', 't', 'r', 'a', 'r', 'y',
        ' ', 't', 'e', 's', 't', ' ', 's', 't', 'r', 'i', 'n', 'g',
    };
    constexpr std::array< std::byte, 16 > reference_hash = {
        std::byte{ 0xFD }, std::byte{ 0xAD }, std::byte{ 0xC7 }, std::byte{ 0xD1 },
        std::byte{ 0x83 }, std::byte{ 0x40 }, std::byte{ 0x95 }, std::byte{ 0xEB },
        std::byte{ 0xB9 }, std::byte{ 0x01 }, std::byte{ 0xC2 }, std::byte{ 0x6F },
        std::byte{ 0xA7 }, std::byte{ 0xB8 }, std::byte{ 0xF1 }, std::byte{ 0xED },
    };

    auto md5 = lcxx::hash::md5( std::span< const uint8_t >{ test_bytes.data(), test_bytes.size() } );
    EXPECT_EQ( md5, reference_hash );
}

TEST( Crypto_Tests, MD5_Hash_Test_whitespace )
{
    constexpr auto                        test_string    = " ";
    constexpr std::array< std::byte, 16 > reference_hash = {
        std::byte{ 0x72 }, std::byte{ 0x15 }, std::byte{ 0xEE }, std::byte{ 0x9C },
        std::byte{ 0x7D }, std::byte{ 0x9D }, std::byte{ 0xC2 }, std::byte{ 0x29 },
        std::byte{ 0xD2 }, std::byte{ 0x92 }, std::byte{ 0x1A }, std::byte{ 0x40 },
        std::byte{ 0xE8 }, std::byte{ 0x99 }, std::byte{ 0xEC }, std::byte{ 0x5F },
    };

    auto md5 = lcxx::hash::md5( test_string );
    EXPECT_EQ( md5, reference_hash );
}