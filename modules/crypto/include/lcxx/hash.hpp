#include <span>

namespace lcxx::hash {

    using md5_hash_t = std::array< std::byte, 16 >;

    auto md5( std::span< std::byte > const input ) -> md5_hash_t;

    template < typename T >
    requires( sizeof( T ) == 1 ) auto md5( std::span< T > const input ) -> md5_hash_t
    {
        return md5( { reinterpret_cast< std::byte * >( input.data() ), input.size() } );
    }

}  // namespace lcxx::hash