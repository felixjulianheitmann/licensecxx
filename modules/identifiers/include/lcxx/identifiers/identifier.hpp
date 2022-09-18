#ifndef LCXX__IDENTIFIERS_IDENTIFIER_HPP__
#define LCXX__IDENTIFIERS_IDENTIFIER_HPP__

#include <string>

namespace lcxx::identifiers {
    struct identifier {
        std::string hash;
        std::string source_text;
    };

}  // namespace lcxx::identifiers

#endif  // LCXX__IDENTIFIERS_IDENTIFIER_HPP__
