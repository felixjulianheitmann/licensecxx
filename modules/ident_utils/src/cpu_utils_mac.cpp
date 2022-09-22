#include <ident_utils/cpu_utils.hpp>

#include <cstring>
#include <sys/sysctl.h>
#include <thread>

#include <ident_utils/common_mac.hpp>

namespace lcxx::ident_utils::cpu {

    auto get_info() -> cpu_info
    {

        auto info = cpu_info;

        info.model_name    = sysctl_str( "machdep.cpu.brand_string" );
        info.n_cores       = sysctl_num( "machdep.cpu.core_count" );
        info.n_threads     = sysctl_num( "machdep.cpu.thread_count" );
        info.vendor        = sysctl_str( "machdep.cpu.vendor" );
        info.max_frequency = sysctl_num( "hw.cpufrequency" );  // Might not return a value

        if ( info.vendor.empty() && info.model_name.starts_with( "Apple" ) ) {
            // Most likely this is an M1 chip
            info.vendor = "Apple";
        }

        return info;
    }
}  // namespace lcxx::ident_utils::cpu
