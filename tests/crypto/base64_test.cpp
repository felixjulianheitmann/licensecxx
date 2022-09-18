#include <gtest/gtest.h>

#include <array>

#include <lcxx/encoding.hpp>

TEST( Crypto_Tests, Base64_Encode_Byte_Test )
{
    constexpr auto const           reference  = "bXkgYXJiaXRyYXJ5IHRlc3Qgc3RyaW5n";
    std::vector< std::byte > const test_bytes = {
        std::byte{ 'm' }, std::byte{ 'y' }, std::byte{ ' ' }, std::byte{ 'a' }, std::byte{ 'r' }, std::byte{ 'b' },
        std::byte{ 'i' }, std::byte{ 't' }, std::byte{ 'r' }, std::byte{ 'a' }, std::byte{ 'r' }, std::byte{ 'y' },
        std::byte{ ' ' }, std::byte{ 't' }, std::byte{ 'e' }, std::byte{ 's' }, std::byte{ 't' }, std::byte{ ' ' },
        std::byte{ 's' }, std::byte{ 't' }, std::byte{ 'r' }, std::byte{ 'i' }, std::byte{ 'n' }, std::byte{ 'g' },
    };
    auto tmp = lcxx::encode::base64( { test_bytes.data(), test_bytes.size() } );

    EXPECT_EQ( tmp, reference );
}

TEST( Crypto_Tests, Base64_Encode_Other_Type_Test )
{
    std::string const                 reference  = "bXkgYXJiaXRyYXJ5IHRlc3Qgc3RyaW5n";
    std::vector< std::uint8_t > const test_bytes = {
        'm', 'y', ' ', 'a', 'r', 'b', 'i', 't', 'r', 'a', 'r', 'y',
        ' ', 't', 'e', 's', 't', ' ', 's', 't', 'r', 'i', 'n', 'g',
    };
    auto tmp = lcxx::encode::base64( std::span< const std::uint8_t >{ test_bytes.data(), test_bytes.size() } );

    EXPECT_EQ( tmp, reference );
}

TEST( Crypto_Tests, Base64_Encode_Other_Type_Long_Test )
{
    std::string const reference =
        "bXkgYXJiaXRyYXJ5dmVyeSBsb25nIHRlc3Qgc3RyaW5nLGFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6YWJjZGVmZ2hpamtsbW5vcHFyc3R1dn"
        "d4eXphYmNkZWZnaGlqa2xtbm9wcXJzdHV2d3h5emFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXph"
        "YmNkZWZnaGlqa2xtbm9wcXJzdHV2d3h5emFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXphYmNkZW"
        "ZnaGlqa2xtbm9wcXJzdHV2d3h5emFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXphYmNkZWZnaGlq"
        "a2xtbm9wcXJzdHV2d3h5eg==";
    std::vector< std::uint8_t > const test_bytes = {
        'm', 'y', ' ', 'a', 'r', 'b', 'i', 't', 'r', 'a', 'r', 'y', 'v', 'e', 'r', 'y', ' ', 'l', 'o', 'n', 'g', ' ',
        't', 'e', 's', 't', ' ', 's', 't', 'r', 'i', 'n', 'g', ',', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'a', 'b',
        'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
        'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
        'u', 'v', 'w', 'x', 'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
        'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'a', 'b', 'c', 'd',
        'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
        's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    };
    auto tmp = lcxx::encode::base64( std::span< const std::uint8_t >{ test_bytes.data(), test_bytes.size() } );

    EXPECT_EQ( tmp, reference );
}

TEST( Crypto_Tests, Base64_Decode_Byte_Test )
{
    std::string const              input     = "bXkgYXJiaXRyYXJ5IHRlc3Qgc3RyaW5n";
    std::vector< std::byte > const reference = {
        std::byte{ 'm' }, std::byte{ 'y' }, std::byte{ ' ' }, std::byte{ 'a' }, std::byte{ 'r' }, std::byte{ 'b' },
        std::byte{ 'i' }, std::byte{ 't' }, std::byte{ 'r' }, std::byte{ 'a' }, std::byte{ 'r' }, std::byte{ 'y' },
        std::byte{ ' ' }, std::byte{ 't' }, std::byte{ 'e' }, std::byte{ 's' }, std::byte{ 't' }, std::byte{ ' ' },
        std::byte{ 's' }, std::byte{ 't' }, std::byte{ 'r' }, std::byte{ 'i' }, std::byte{ 'n' }, std::byte{ 'g' },
    };
    auto tmp = lcxx::decode::base64( input );

    EXPECT_EQ( tmp, reference );
}

TEST( Crypto_Tests, Base64_Decode_Byte_Long_Test )
{
    std::string const reference =
        "bXkgYXJiaXRyYXJ5dmVyeSBsb25nIHRlc3Qgc3RyaW5nLGFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6YWJjZGVmZ2hpamtsbW5vcHFyc3R1dn"
        "d4eXphYmNkZWZnaGlqa2xtbm9wcXJzdHV2d3h5emFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXph"
        "YmNkZWZnaGlqa2xtbm9wcXJzdHV2d3h5emFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXphYmNkZW"
        "ZnaGlqa2xtbm9wcXJzdHV2d3h5emFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXphYmNkZWZnaGlq"
        "a2xtbm9wcXJzdHV2d3h5eg==";
    std::vector< std::byte > const test_bytes = {
        std::byte{ 'm' }, std::byte{ 'y' }, std::byte{ ' ' }, std::byte{ 'a' }, std::byte{ 'r' }, std::byte{ 'b' },
        std::byte{ 'i' }, std::byte{ 't' }, std::byte{ 'r' }, std::byte{ 'a' }, std::byte{ 'r' }, std::byte{ 'y' },
        std::byte{ 'v' }, std::byte{ 'e' }, std::byte{ 'r' }, std::byte{ 'y' }, std::byte{ ' ' }, std::byte{ 'l' },
        std::byte{ 'o' }, std::byte{ 'n' }, std::byte{ 'g' }, std::byte{ ' ' }, std::byte{ 't' }, std::byte{ 'e' },
        std::byte{ 's' }, std::byte{ 't' }, std::byte{ ' ' }, std::byte{ 's' }, std::byte{ 't' }, std::byte{ 'r' },
        std::byte{ 'i' }, std::byte{ 'n' }, std::byte{ 'g' }, std::byte{ ',' }, std::byte{ 'a' }, std::byte{ 'b' },
        std::byte{ 'c' }, std::byte{ 'd' }, std::byte{ 'e' }, std::byte{ 'f' }, std::byte{ 'g' }, std::byte{ 'h' },
        std::byte{ 'i' }, std::byte{ 'j' }, std::byte{ 'k' }, std::byte{ 'l' }, std::byte{ 'm' }, std::byte{ 'n' },
        std::byte{ 'o' }, std::byte{ 'p' }, std::byte{ 'q' }, std::byte{ 'r' }, std::byte{ 's' }, std::byte{ 't' },
        std::byte{ 'u' }, std::byte{ 'v' }, std::byte{ 'w' }, std::byte{ 'x' }, std::byte{ 'y' }, std::byte{ 'z' },
        std::byte{ 'a' }, std::byte{ 'b' }, std::byte{ 'c' }, std::byte{ 'd' }, std::byte{ 'e' }, std::byte{ 'f' },
        std::byte{ 'g' }, std::byte{ 'h' }, std::byte{ 'i' }, std::byte{ 'j' }, std::byte{ 'k' }, std::byte{ 'l' },
        std::byte{ 'm' }, std::byte{ 'n' }, std::byte{ 'o' }, std::byte{ 'p' }, std::byte{ 'q' }, std::byte{ 'r' },
        std::byte{ 's' }, std::byte{ 't' }, std::byte{ 'u' }, std::byte{ 'v' }, std::byte{ 'w' }, std::byte{ 'x' },
        std::byte{ 'y' }, std::byte{ 'z' }, std::byte{ 'a' }, std::byte{ 'b' }, std::byte{ 'c' }, std::byte{ 'd' },
        std::byte{ 'e' }, std::byte{ 'f' }, std::byte{ 'g' }, std::byte{ 'h' }, std::byte{ 'i' }, std::byte{ 'j' },
        std::byte{ 'k' }, std::byte{ 'l' }, std::byte{ 'm' }, std::byte{ 'n' }, std::byte{ 'o' }, std::byte{ 'p' },
        std::byte{ 'q' }, std::byte{ 'r' }, std::byte{ 's' }, std::byte{ 't' }, std::byte{ 'u' }, std::byte{ 'v' },
        std::byte{ 'w' }, std::byte{ 'x' }, std::byte{ 'y' }, std::byte{ 'z' }, std::byte{ 'a' }, std::byte{ 'b' },
        std::byte{ 'c' }, std::byte{ 'd' }, std::byte{ 'e' }, std::byte{ 'f' }, std::byte{ 'g' }, std::byte{ 'h' },
        std::byte{ 'i' }, std::byte{ 'j' }, std::byte{ 'k' }, std::byte{ 'l' }, std::byte{ 'm' }, std::byte{ 'n' },
        std::byte{ 'o' }, std::byte{ 'p' }, std::byte{ 'q' }, std::byte{ 'r' }, std::byte{ 's' }, std::byte{ 't' },
        std::byte{ 'u' }, std::byte{ 'v' }, std::byte{ 'w' }, std::byte{ 'x' }, std::byte{ 'y' }, std::byte{ 'z' },
        std::byte{ 'a' }, std::byte{ 'b' }, std::byte{ 'c' }, std::byte{ 'd' }, std::byte{ 'e' }, std::byte{ 'f' },
        std::byte{ 'g' }, std::byte{ 'h' }, std::byte{ 'i' }, std::byte{ 'j' }, std::byte{ 'k' }, std::byte{ 'l' },
        std::byte{ 'm' }, std::byte{ 'n' }, std::byte{ 'o' }, std::byte{ 'p' }, std::byte{ 'q' }, std::byte{ 'r' },
        std::byte{ 's' }, std::byte{ 't' }, std::byte{ 'u' }, std::byte{ 'v' }, std::byte{ 'w' }, std::byte{ 'x' },
        std::byte{ 'y' }, std::byte{ 'z' }, std::byte{ 'a' }, std::byte{ 'b' }, std::byte{ 'c' }, std::byte{ 'd' },
        std::byte{ 'e' }, std::byte{ 'f' }, std::byte{ 'g' }, std::byte{ 'h' }, std::byte{ 'i' }, std::byte{ 'j' },
        std::byte{ 'k' }, std::byte{ 'l' }, std::byte{ 'm' }, std::byte{ 'n' }, std::byte{ 'o' }, std::byte{ 'p' },
        std::byte{ 'q' }, std::byte{ 'r' }, std::byte{ 's' }, std::byte{ 't' }, std::byte{ 'u' }, std::byte{ 'v' },
        std::byte{ 'w' }, std::byte{ 'x' }, std::byte{ 'y' }, std::byte{ 'z' }, std::byte{ 'a' }, std::byte{ 'b' },
        std::byte{ 'c' }, std::byte{ 'd' }, std::byte{ 'e' }, std::byte{ 'f' }, std::byte{ 'g' }, std::byte{ 'h' },
        std::byte{ 'i' }, std::byte{ 'j' }, std::byte{ 'k' }, std::byte{ 'l' }, std::byte{ 'm' }, std::byte{ 'n' },
        std::byte{ 'o' }, std::byte{ 'p' }, std::byte{ 'q' }, std::byte{ 'r' }, std::byte{ 's' }, std::byte{ 't' },
        std::byte{ 'u' }, std::byte{ 'v' }, std::byte{ 'w' }, std::byte{ 'x' }, std::byte{ 'y' }, std::byte{ 'z' },
        std::byte{ 'a' }, std::byte{ 'b' }, std::byte{ 'c' }, std::byte{ 'd' }, std::byte{ 'e' }, std::byte{ 'f' },
        std::byte{ 'g' }, std::byte{ 'h' }, std::byte{ 'i' }, std::byte{ 'j' }, std::byte{ 'k' }, std::byte{ 'l' },
        std::byte{ 'm' }, std::byte{ 'n' }, std::byte{ 'o' }, std::byte{ 'p' }, std::byte{ 'q' }, std::byte{ 'r' },
        std::byte{ 's' }, std::byte{ 't' }, std::byte{ 'u' }, std::byte{ 'v' }, std::byte{ 'w' }, std::byte{ 'x' },
        std::byte{ 'y' }, std::byte{ 'z' }, std::byte{ 'a' }, std::byte{ 'b' }, std::byte{ 'c' }, std::byte{ 'd' },
        std::byte{ 'e' }, std::byte{ 'f' }, std::byte{ 'g' }, std::byte{ 'h' }, std::byte{ 'i' }, std::byte{ 'j' },
        std::byte{ 'k' }, std::byte{ 'l' }, std::byte{ 'm' }, std::byte{ 'n' }, std::byte{ 'o' }, std::byte{ 'p' },
        std::byte{ 'q' }, std::byte{ 'r' }, std::byte{ 's' }, std::byte{ 't' }, std::byte{ 'u' }, std::byte{ 'v' },
        std::byte{ 'w' }, std::byte{ 'x' }, std::byte{ 'y' }, std::byte{ 'z' }, std::byte{ 'a' }, std::byte{ 'b' },
        std::byte{ 'c' }, std::byte{ 'd' }, std::byte{ 'e' }, std::byte{ 'f' }, std::byte{ 'g' }, std::byte{ 'h' },
        std::byte{ 'i' }, std::byte{ 'j' }, std::byte{ 'k' }, std::byte{ 'l' }, std::byte{ 'm' }, std::byte{ 'n' },
        std::byte{ 'o' }, std::byte{ 'p' }, std::byte{ 'q' }, std::byte{ 'r' }, std::byte{ 's' }, std::byte{ 't' },
        std::byte{ 'u' }, std::byte{ 'v' }, std::byte{ 'w' }, std::byte{ 'x' }, std::byte{ 'y' }, std::byte{ 'z' },
        std::byte{ 'a' }, std::byte{ 'b' }, std::byte{ 'c' }, std::byte{ 'd' }, std::byte{ 'e' }, std::byte{ 'f' },
        std::byte{ 'g' }, std::byte{ 'h' }, std::byte{ 'i' }, std::byte{ 'j' }, std::byte{ 'k' }, std::byte{ 'l' },
        std::byte{ 'm' }, std::byte{ 'n' }, std::byte{ 'o' }, std::byte{ 'p' }, std::byte{ 'q' }, std::byte{ 'r' },
        std::byte{ 's' }, std::byte{ 't' }, std::byte{ 'u' }, std::byte{ 'v' }, std::byte{ 'w' }, std::byte{ 'x' },
        std::byte{ 'y' }, std::byte{ 'z' }, std::byte{ 'a' }, std::byte{ 'b' }, std::byte{ 'c' }, std::byte{ 'd' },
        std::byte{ 'e' }, std::byte{ 'f' }, std::byte{ 'g' }, std::byte{ 'h' }, std::byte{ 'i' }, std::byte{ 'j' },
        std::byte{ 'k' }, std::byte{ 'l' }, std::byte{ 'm' }, std::byte{ 'n' }, std::byte{ 'o' }, std::byte{ 'p' },
        std::byte{ 'q' }, std::byte{ 'r' }, std::byte{ 's' }, std::byte{ 't' }, std::byte{ 'u' }, std::byte{ 'v' },
        std::byte{ 'w' }, std::byte{ 'x' }, std::byte{ 'y' }, std::byte{ 'z' },
    };

    auto tmp = lcxx::encode::base64( std::span< const std::byte >{ test_bytes.data(), test_bytes.size() } );

    EXPECT_EQ( tmp, reference );
}
