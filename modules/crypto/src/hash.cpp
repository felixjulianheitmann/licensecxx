#include <lcxx/hash.hpp>

#include <array>
#include <cstddef>

namespace lcxx::hash {

    auto md5( std::string const & input ) -> md5_hash_t
    {
        std::span< const std::byte > const byte_span = { reinterpret_cast< std::byte const * >( input.data() ),
                                                         input.size() };
        return md5( byte_span );
    }

    auto md5( std::span< const std::byte > const input ) -> md5_hash_t
    {
        md5_hash_t hash_buffer;
        MD5( reinterpret_cast< unsigned char const * >( input.data() ), input.size(),
             reinterpret_cast< unsigned char * >( hash_buffer.data() ) );

        return hash_buffer;
    }

    auto sha512( std::string const & input ) -> sha512_hash_t
    {
        std::span< const std::byte > const byte_span = { reinterpret_cast< std::byte const * >( input.data() ),
                                                         input.size() };
        return sha512( byte_span );
    }

    auto sha512( std::span< const std::byte > const input ) -> sha512_hash_t
    {
        sha512_hash_t hash_buffer;
        SHA512( reinterpret_cast< unsigned char const * >( input.data() ), input.size(),
                reinterpret_cast< unsigned char * >( hash_buffer.data() ) );

        return hash_buffer;
    }

}  // namespace lcxx::hash
