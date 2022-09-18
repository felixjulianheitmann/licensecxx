#ifndef LCXX__OS_UTILS_DISK_UTILS_HPP__
#define LCXX__OS_UTILS_DISK_UTILS_HPP__

#include <string>

namespace lcxx::os::disk {

    struct disk_info {
        std::size_t size;
        std::string volume_id;
    };

    auto get_info() -> disk_info;

}  // namespace lcxx::os::disk

#endif  // LCXX__OS_UTILS_DISK_UTILS_HPP__
