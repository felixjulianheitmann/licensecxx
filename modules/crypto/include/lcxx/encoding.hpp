#ifndef LCXX__CRYPTO_ENCODING_HPP__
#define LCXX__CRYPTO_ENCODING_HPP__

#include <cstddef>
#include <span>
#include <string>
#include <vector>

namespace lcxx::encode {

    /**
     * @brief encodes a byte span to a base64 string
     *
     * @param bytes the byte span that will be encoded
     * @return std::string the input bytes as base64 encoded string
     */
    auto base64( std::span< const std::byte > const bytes ) -> std::string;

    template < typename T >
    requires( sizeof( T ) == 1 ) auto base64( std::span< const T > const bytes ) -> std::string
    {
        return base64( { reinterpret_cast< std::byte const * >( bytes.data() ), bytes.size() } );
    }

}  // namespace lcxx::encode

namespace lcxx::decode {

    /**
     * @brief takes a base64 encoded string and returns the contained bytes as a vector
     *
     * @param input a string representing base64 encoded data
     * @return std::vector< std::byte > the input data in byte form
     */
    auto base64( std::string const & input ) -> std::vector< std::byte >;

}  // namespace lcxx::decode

#endif  // LCXX__CRYPTO_ENCODING_HPP__
