#include <ident_utils/cpu_utils.hpp>

#include <stdexcept>

namespace lcxx::experimental::ident_utils::cpu {

    auto get_info() -> cpu_info
    {
        throw std::runtime_error( "This feature is not yet implemented." );
        return {};
    }

}  // namespace lcxx::experimental::ident_utils::cpu
