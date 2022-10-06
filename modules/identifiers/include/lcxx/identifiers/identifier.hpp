#ifndef LCXX__IDENTIFIERS_IDENTIFIER_HPP__
#define LCXX__IDENTIFIERS_IDENTIFIER_HPP__

#include <string>

namespace lcxx::experimental::identifiers {
    struct identifier {
        std::string hash;
        std::string source_text;
    };

}  // namespace lcxx::experimental::identifiers

#endif  // LCXX__IDENTIFIERS_IDENTIFIER_HPP__
