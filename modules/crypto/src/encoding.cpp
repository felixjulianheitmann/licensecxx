#include <lcxx/encoding.hpp>

#include <vector>

#include <openssl/evp.h>

namespace lcxx::encode {
    auto base64( std::span< const std::byte > const bytes ) -> std::string
    {
        // Add additional 4 bytes as buffer, because the rounding would just make it more complicated.
        std::vector< unsigned char > encoded( bytes.size() * 4 / 3 + 4 + 1, 0 );
        auto                         true_size =
            EVP_EncodeBlock( encoded.data(), reinterpret_cast< unsigned char const * >( bytes.data() ), bytes.size() );

        // Do not include terminating null-character
        return std::string{ encoded.begin(), encoded.begin() + true_size };
    }

}  // namespace lcxx::encode

namespace lcxx::decode {

    auto base64( std::string const & input ) -> std::vector< std::byte >
    {

        std::vector< std::byte > decoded( input.size() * 3 / 4, std::byte{ 0 } );

        EVP_DecodeBlock( reinterpret_cast< unsigned char * >( decoded.data() ),
                         reinterpret_cast< unsigned char const * >( input.data() ), input.size() );

        // Remove empty bytes at the end
        if ( input.size() >= 1 && input.back() == '=' )
            decoded.pop_back();
        if ( input.size() >= 2 && *( input.end() - 2 ) == '=' )
            decoded.pop_back();
        return decoded;
    }

}  // namespace lcxx::decode
