#include <gtest/gtest.h>

#include <array>

TEST( MD5_Hash_Test, Crypto_Tests )
{
    constexpr auto                        test_string    = "my arbitrary test string";
    constexpr std::array< std::byte, 16 > reference_hash = {
        std::byte{ 0xFD }, std::byte{ 0xAD }, std::byte{ 0xC7 }, std::byte{ 0xD1 },
        std::byte{ 0x83 }, std::byte{ 0x40 }, std::byte{ 0x95 }, std::byte{ 0xEB },
        std::byte{ 0xB9 }, std::byte{ 0x01 }, std::byte{ 0xC2 }, std::byte{ 0x6F },
        std::byte{ 0xA7 }, std::byte{ 0xB8 }, std::byte{ 0xF1 }, std::byte{ 0xED },
    };
}