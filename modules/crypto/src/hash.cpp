#include <lcxx/hash.hpp>

#include <openssl/md5.h>

namespace lcxx::hash {

    auto md5( std::span< std::byte > const input ) -> md5_hash_t
    {
        md5_hash_t hash_buffer;
        MD5( reinterpret_cast< unsigned char * >( input.data() ), input.size(),
             reinterpret_cast< unsigned char * >( hash_buffer.data() ) );

        return hash_buffer;
    }

}  // namespace lcxx::hash
