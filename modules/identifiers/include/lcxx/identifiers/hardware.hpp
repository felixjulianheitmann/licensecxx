#ifndef LCXX__IDENTIFIERS_HARDWARE_HPP__
#define LCXX__IDENTIFIERS_HARDWARE_HPP__

#include <cstddef>
#include <string>
#include <utility>

#include <lcxx/identifiers/identifier.hpp>

namespace lcxx::identifiers {

    enum class hw_ident_strat : std::int32_t {
        all      = 1,
        cpu      = 1 << 1,
        gpu      = 1 << 2,
        ethernet = 1 << 3,
    };

    /**
     * @brief analysis the host machine and creates an identification string from the gathered information. The analyzed
     * aspects depend on the strategy chosen
     *
     * @param strategy the analyzed hardware aspects. Different strategies can be combined through bitwise-or|
     * @return identifier the identifier containing a hash and the clear text result of the hardware analysis
     */
    auto hardware( hw_ident_strat const strategy = hw_ident_strat::all ) -> identifier;

}  // namespace lcxx::identifiers

#endif  // LCXX__IDENTIFIERS_HARDWARE_HPP__
