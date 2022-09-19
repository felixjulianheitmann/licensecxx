#ifndef LCXX__OS_UTILS_OS_UTILS_HPP__
#define LCXX__OS_UTILS_OS_UTILS_HPP__

#include <string>
#include <vector>

namespace lcxx::os::os {

    struct os_info {
        std::string                os_name;
        std::string                architecture;
        std::string                pc_name;
        std::string                user;
        std::vector< std::string > groups;
        std::size_t                uid;
        std::vector< std::size_t > gids;
    };

    auto get_info() -> os_info;

}  // namespace lcxx::os::os

#endif  // LCXX__OS_UTILS_OS_UTILS_HPP__
