#include <ident_utils/cpu_utils.hpp>

#include <sys/sysinfo.h>
#include <thread>

#include <ident_utils/common.hpp>

namespace lcxx::ident_utils::cpu {

    auto get_info() -> cpu_info
    {
        cpu_info ci;

        ci.max_frequency = std::stoull( cat_file( "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq" ) );
        ci.n_threads     = std::thread::hardware_concurrency();
        auto cpu_info    = cat_file( "/proc/cpuinfo" );

        auto parse_cpu_info_attr = []( std::string const & info, std::string_view const key ) -> std::string {
            std::string_view info_v = info;
            if ( auto pos = info_v.find( key ); pos != std::string::npos ) {
                info_v.remove_prefix( pos );
                info_v.remove_prefix( info_v.find( ":" ) + 1 );
                info_v.remove_suffix( info_v.size() - info_v.find( '\n' ) );
                return std::string{ info_v };
            }

            return {};
        };

        ci.vendor     = parse_cpu_info_attr( cpu_info, "vendor_id" );
        ci.model_name = parse_cpu_info_attr( cpu_info, "model name" );
        ci.n_cores    = std::stoull( parse_cpu_info_attr( cpu_info, "cpu cores" ) );

        return ci;
    }

}  // namespace lcxx::ident_utils::cpu
