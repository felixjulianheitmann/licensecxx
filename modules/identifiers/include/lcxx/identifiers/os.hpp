#ifndef LCXX__IDENTIFIERS_USER_HPP__
#define LCXX__IDENTIFIERS_USER_HPP__

#include <cstddef>
#include <string>
#include <utility>

#include <lcxx/identifiers/identifier.hpp>

namespace lcxx::experimental::identifiers {

    enum class os_ident_strat : ident_strat_t {
        all             = 1,
        os_name         = 1 << 1,
        os_architecture = 1 << 2,
        os_pc_name      = 1 << 3,
        user_name       = 1 << 11,
        user_uid        = 1 << 12,
        user_groups     = 1 << 13,
        user_gids       = 1 << 14,
        os              = os_name | os_architecture | os_pc_name,
        user            = user_name | user_uid | user_groups | user_gids,

    };

    auto operator|( os_ident_strat const lhs, os_ident_strat const rhs ) -> os_ident_strat;

    /**
     * @brief polls current os info from the operating system and generates an identifier based on the chosen os
     * identification strategy
     *
     * @param strategy the analyzed os features. Different strategies can be combined through bitwise-or|
     * @return identifier the identifier containing a hash and the clear text result of the os analysis
     */
    auto os( os_ident_strat const strategy = os_ident_strat::all ) -> identifier;

    /**
     * @brief verifies a os identification hash against the os this software is run on with respect to the
     * identification strategy chosen
     *
     * @param strategy the strategy chosen to identify os info. Must match the strategy used to generate the hash
     * @param hash the hash produced by a call to `os()`
     * @return true if the current os environment matches the one used to generate the hash
     * @return false otherwise
     */
    auto verify( os_ident_strat const strategy, std::string_view const hash ) -> bool;

}  // namespace lcxx::experimental::identifiers

#endif  // LCXX__IDENTIFIERS_USER_HPP__