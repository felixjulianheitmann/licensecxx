#include <string>

namespace lcxx::os::disk {

    struct disk_info {
        std::size_t size;
        std::string volume_id;
    };

    auto get_info() -> disk_info;

}  // namespace lcxx::os::disk
