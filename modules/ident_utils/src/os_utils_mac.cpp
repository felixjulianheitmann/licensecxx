#include <ident_utils/os_utils.hpp>

#include <sys/unistd.h>
#include <sys/utsname.h>

#include <ident_utils/common_mac.hpp>

namespace lcxx::ident_utils::os {

    auto get_info() -> os_info
    {
        auto info = os_info;

        info.os_pc_name = sysctl_str( "kern.hostname" );
        info.

            return info;
    }

}  // namespace lcxx::ident_utils::os
