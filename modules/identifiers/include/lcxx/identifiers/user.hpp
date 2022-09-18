#include <cstddef>
#include <string>
#include <utility>

#include <lcxx/identifiers/identifier.hpp>

namespace lcxx::identifiers {

    enum class user_ident_strat : std::int32_t {
        all              = 1,
        name             = 1 << 1,
        admin_privileges = 1 << 2,
    };

    /**
     * @brief polls current user info from the operating system and generates an identifier based on the chosen user
     * identification strategy
     *
     * @param strategy the analyzed user features. Different strategies can be combined through bitwise-or|
     * @return identifier the identifier containing a hash and the clear text result of the user analysis
     */
    auto user( user_ident_strat const strategy ) -> identifier;

}  // namespace lcxx::identifiers
