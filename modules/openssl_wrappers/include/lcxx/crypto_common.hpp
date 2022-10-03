#ifndef LCXX__CRYPTO_COMMON_HPP__
#define LCXX__CRYPTO_COMMON_HPP__

#include <ranges>

namespace lcxx::crypto::detail {

    template < typename B >
    concept contiguous_byte_range = std::ranges::contiguous_range< B > && requires( B bytes )
    {
        requires( std::is_trivially_copyable< std::remove_cvref_t< std::ranges::range_value_t< B > > >() == true &&
                  sizeof( std::ranges::range_value_t< std::remove_cvref_t< B > > ) == 1 );
    };

}  // namespace lcxx::crypto::detail

#endif  // LCXX__CRYPTO_COMMON_HPP__