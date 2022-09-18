#ifndef LCXX__CRYPTO_ENCODING_HPP__
#define LCXX__CRYPTO_ENCODING_HPP__

#include <cstddef>
#include <span>
#include <string>
#include <vector>

namespace lcxx::encode {
    auto base64( std::span< const std::byte > const bytes ) -> std::string;

    template < typename T >
    requires( sizeof( T ) == 1 ) auto base64( std::span< const T > const bytes ) -> std::string
    {
        return base64( { reinterpret_cast< std::byte const * >( bytes.data() ), bytes.size() } );
    }

}  // namespace lcxx::encode

namespace lcxx::decode {

    auto base64( std::string const & input ) -> std::vector< std::byte >;

}  // namespace lcxx::decode

#endif  // LCXX__CRYPTO_ENCODING_HPP__
