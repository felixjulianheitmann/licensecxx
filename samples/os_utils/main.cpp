#include <iostream>

#include <nlohmann/json.hpp>

#include <os_utils/cpu_utils.hpp>
#include <os_utils/disk_utils.hpp>

using namespace lcxx::os;

namespace lcxx::os::cpu {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( cpu_info, n_cores, max_frequency );
}
namespace lcxx::os::disk {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( disk_info, size, volume_id );
}

auto main() -> int
{
    auto print = []( auto const & desc, auto const & info ) {
        std::cout << desc << ": " << nlohmann::json( info ).dump( 4 ) << std::endl;
    };

    print( "cpu", cpu::get_info() );
    print( "disk", disk::get_info() );

    return 0;
}