#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <ranges>

#include <lcxx/hash.hpp>

template < std::ranges::range R >
auto str_to_vec( std::string_view const str ) -> R
    requires( sizeof( std::ranges::range_value_t< R > ) == 1 )
{
    auto bytes = str | std::views::transform( []( auto && c ) {
                     return *reinterpret_cast< std::ranges::range_value_t< R > const * >( &c );
                 } );
    return { bytes.begin(), bytes.end() };
}

template < typename T, std::size_t N >
auto gen_byte_arr( std::initializer_list< T > const & r ) -> std::array< std::byte, N >
{
    auto bytes = r | std::views::transform( []( auto && c ) { return std::byte{ c }; } );

    std::array< std::byte, N > arr;
    std::ranges::copy( bytes, arr.begin() );
    return arr;
}

constexpr auto    test_string    = "my arbitrary test string";
static const auto reference_hash = gen_byte_arr< uint8_t, 64 >( {
    0xb2, 0xd1, 0xdc, 0x2a, 0xd5, 0x61, 0xc7, 0x2b, 0x68, 0x53, 0x6c, 0xa6, 0x34, 0x5b, 0x2e, 0xd2,
    0xeb, 0xba, 0x6e, 0x97, 0x2a, 0x0b, 0x71, 0x6c, 0x03, 0xdc, 0x44, 0xc7, 0xaa, 0xcd, 0x02, 0xa7,
    0x89, 0x9e, 0xc1, 0x7a, 0xca, 0x2a, 0x3d, 0xeb, 0xc2, 0xa5, 0xd5, 0x70, 0xdb, 0xeb, 0x8e, 0x9c,
    0xd6, 0xbf, 0xa4, 0xe5, 0x19, 0xe5, 0xd0, 0xe6, 0x56, 0xda, 0x06, 0x62, 0xc3, 0x0b, 0xc8, 0x2b,
} );

TEST( Crypto_Tests, SHA512_Hash_String_Test )
{
    auto [sha512, err] = lcxx::hash::sha512( test_string );
    EXPECT_EQ( err, lcxx::hash::error::ok );
    EXPECT_EQ( sha512, reference_hash );
}

TEST( Crypto_Tests, SHA512_Hash_Byte_Test )
{
    auto const test_bytes = str_to_vec< std::vector< std::byte > >( test_string );
    auto [sha512, err]    = lcxx::hash::sha512( test_bytes );
    EXPECT_EQ( err, lcxx::hash::error::ok );
    EXPECT_EQ( sha512, reference_hash );
}

TEST( Crypto_Tests, SHA512_Hash_Other_Type_Test )
{
    auto const test_bytes = str_to_vec< std::vector< uint8_t > >( test_string );
    auto [sha512, err]    = lcxx::hash::sha512( std::span< const uint8_t >{ test_bytes.data(), test_bytes.size() } );
    EXPECT_EQ( err, lcxx::hash::error::ok );
    EXPECT_EQ( sha512, reference_hash );
}

TEST( Crypto_Tests, SHA512_Hash_Test_whitespace )
{
    constexpr auto                        test_string    = " ";
    auto                                  reference_hash = gen_byte_arr< uint8_t, 64 >( {
        0xf9, 0x0d, 0xdd, 0x77, 0xe4, 0x00, 0xdf, 0xe6, 0xa3, 0xfc, 0xf4, 0x79, 0xb0, 0x0b, 0x1e, 0xe2,
        0x9e, 0x70, 0x15, 0xc5, 0xbb, 0x8c, 0xd7, 0x0f, 0x5f, 0x15, 0xb4, 0x88, 0x6c, 0xc3, 0x39, 0x27,
        0x5f, 0xf5, 0x53, 0xfc, 0x8a, 0x05, 0x3f, 0x8d, 0xdc, 0x73, 0x24, 0xf4, 0x51, 0x68, 0xcf, 0xfa,
        0xf8, 0x1f, 0x8c, 0x3a, 0xc9, 0x39, 0x96, 0xf6, 0x53, 0x6e, 0xef, 0x38, 0xe5, 0xe4, 0x07, 0x68,
    } );

     auto [sha512, err] = lcxx::hash::sha512( test_string );
     EXPECT_EQ( err, lcxx::hash::error::ok );
     EXPECT_EQ( sha512, reference_hash );
}