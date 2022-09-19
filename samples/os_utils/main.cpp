#include <iostream>

#include <nlohmann/json.hpp>

#include <os_utils/cpu_utils.hpp>
#include <os_utils/disk_utils.hpp>
#include <os_utils/os_utils.hpp>

using namespace lcxx::os;

namespace lcxx::os::cpu {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( cpu_info, vendor, model_name, n_cores, n_threads, max_frequency );
}
namespace lcxx::os::disk {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( disk_info, size, volume_name, uuid );
}
namespace lcxx::os::os {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( os_info, os_name, architecture, pc_name, user, groups, uid, gids );
}

auto main() -> int
{
    auto print = []( auto const & desc, auto const & info ) {
        std::cout << desc << ": " << nlohmann::json( info ).dump( 4 ) << std::endl;
    };

    print( "cpu", cpu::get_info() );
    print( "os", os::get_info() );
    // print( "disk", disk::get_info() );

    return 0;
}