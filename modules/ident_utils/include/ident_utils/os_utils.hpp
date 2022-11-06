#ifndef LCXX__IDENT_UTILS_OS_UTILS_HPP__
#define LCXX__IDENT_UTILS_OS_UTILS_HPP__

#include <string>
#include <vector>

namespace lcxx::experimental::ident_utils::os {

    struct os_info {
        std::string                os_name;
        std::string                os_architecture;
        std::string                os_pc_name;
        std::string                user_name;
        std::vector< std::string > user_groups;
        std::size_t                user_uid;
        std::vector< std::size_t > user_gids;
    };

    /**
     * @brief retrieves information about the CPU built into this machine
     *
     * @return os_info an information object about the OS
     */
    auto get_info() -> os_info;

}  // namespace lcxx::experimental::ident_utils::os

#endif  // LCXX__IDENT_UTILS_OS_UTILS_HPP__
