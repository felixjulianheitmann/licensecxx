#ifndef LCXX__OS_UTILS_OS_UTILS_HPP__
#define LCXX__OS_UTILS_OS_UTILS_HPP__

#include <string>
#include <vector>

namespace lcxx::ident_utils::os {

    struct os_info {
        std::string                os_name;
        std::string                os_architecture;
        std::string                os_pc_name;
        std::string                user_name;
        std::vector< std::string > user_groups;
        std::size_t                user_uid;
        std::vector< std::size_t > user_gids;
    };

    auto get_info() -> os_info;

}  // namespace lcxx::ident_utils::os

#endif  // LCXX__OS_UTILS_OS_UTILS_HPP__
