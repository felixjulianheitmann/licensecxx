#ifndef LCXX__IDENTIFIERS_COMMON_HPP__
#define LCXX__IDENTIFIERS_COMMON_HPP__

#include <concepts>

template < typename Strat_Enum >

requires( std::is_enum< Strat_Enum >::value ) auto strat_active( Strat_Enum strat, Strat_Enum query ) -> bool
{
    using base_t = std::underlying_type_t< Strat_Enum >;
    return static_cast< base_t >( strat ) & static_cast< base_t >( query );
};

#endif  // LCXX__IDENTIFIERS_COMMON_HPP__