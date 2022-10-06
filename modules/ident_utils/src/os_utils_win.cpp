#include <ident_utils/os_utils.hpp>

#include <stdexcept>

namespace lcxx::experimental::ident_utils::os {

    auto get_info() -> os_info
    {
        throw std::runtime_error( "This feature is not yet implemented." );
        return {};
    }

}  // namespace lcxx::experimental::ident_utils::os
