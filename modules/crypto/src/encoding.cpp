#include <lcxx/encoding.hpp>

#include <vector>

#include <openssl/evp.h>

namespace lcxx::encode {
    auto base64( std::span< std::byte > const bytes ) -> std::string
    {
        // std::vector< unsigned char > encoded( 1, 0 );
        // std::size_t                  bytes_processed = 0;
        // while ( bytes_processed < bytes.size() ) {
        //     auto        bytes_left = bytes.size() - bytes_processed;
        //     std::size_t block_size = bytes_left;
        //     if ( bytes_left > 48 ) {
        //         block_size = 48;
        //     }

        //     encoded.resize( encoded.size() + 65 );

        //     bytes_processed += EVP_EncodeBlock(
        //         encoded.data() + encoded.size() - 65,
        //         reinterpret_cast< unsigned char const * >( bytes.data() ) + bytes_processed, block_size );
        // }

        // calculate no. of characters by multiplying bytes.size() * 4/3. Add 2 to always round up on division. Add for
        // terminating null character
        std::vector< unsigned char > encoded( ( bytes.size() * 4 + 2 ) / 3 + 1, 0 );
        EVP_EncodeBlock( encoded.data(), reinterpret_cast< unsigned char const * >( bytes.data() ), bytes.size() );

        return std::string{ encoded.begin(), encoded.end() };
    }

}  // namespace lcxx::encode

namespace lcxx::decode {

    auto base64( std::string const & input ) -> std::vector< std::byte >
    {

        std::vector< std::byte > decoded( input.size() * 3 / 4, std::byte{ 0 } );
        EVP_EncodeBlock( reinterpret_cast< unsigned char * >( decoded.data() ),
                         reinterpret_cast< unsigned char const * >( input.data() ), input.size() );

        return decoded;
    }

}  // namespace lcxx::decode
