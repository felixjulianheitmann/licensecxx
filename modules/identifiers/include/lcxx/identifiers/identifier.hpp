#ifndef LCXX__IDENTIFIERS_IDENTIFIER_HPP__
#define LCXX__IDENTIFIERS_IDENTIFIER_HPP__

#include <string>

namespace lcxx::experimental::identifiers {

    using ident_strat_t = std::int32_t;

    struct identifier {
        std::string   hash;
        std::string   source_text;
        ident_strat_t ident_strat;
    };

}  // namespace lcxx::experimental::identifiers

#endif  // LCXX__IDENTIFIERS_IDENTIFIER_HPP__
