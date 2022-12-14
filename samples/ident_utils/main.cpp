#include <iostream>

#include <nlohmann/json.hpp>

#include <ident_utils/cpu_utils.hpp>
#include <ident_utils/os_utils.hpp>

using namespace lcxx::experimental::ident_utils;

namespace lcxx::experimental::ident_utils::cpu {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( cpu_info, vendor, model_name, n_cores, n_threads, max_frequency );
}
namespace lcxx::experimental::ident_utils::os {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( os_info, os_name, os_architecture, os_pc_name, user_name, user_groups, user_uid,
                                        user_gids );
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