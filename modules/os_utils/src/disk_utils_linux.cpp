#include <os_utils/disk_utils.hpp>

#include <memory>

#include <os_utils/common.hpp>

namespace lcxx::os::disk {

    auto get_info() -> disks_info
    {
        throw std::runtime_error( "Not yet implemented." );

        disks_info di;
        return di;
    }

}  // namespace lcxx::os::disk
