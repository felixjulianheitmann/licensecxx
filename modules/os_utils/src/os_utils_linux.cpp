#include <os_utils/os_utils.hpp>

#include <algorithm>
#include <grp.h>
#include <ranges>
#include <sys/unistd.h>
#include <sys/utsname.h>

#include <os_utils/common.hpp>

namespace lcxx::os::os {

    auto get_info() -> os_info
    {
        os_info oi;

        oi.user = c2s_wrapper( []( auto buffer, auto len ) { getlogin_r( buffer, len ); } );
        oi.uid  = getuid();

        int                  buffer_len = 64;
        std::vector< gid_t > group_ids( buffer_len, 0 );
        getgrouplist( oi.user.c_str(), getgid(), group_ids.data(), &buffer_len );
        std::ranges::transform( group_ids | std::views::take( buffer_len ), std::back_inserter( oi.gids ),
                                []( auto const id ) -> std::size_t { return id; } );
        std::ranges::sort( oi.gids );

        for ( auto const gid : oi.gids ) {
            c2s_wrapper( [&]( auto buffer, auto len ) {
                struct group   g;
                struct group * g_ptr;
                getgrgid_r( gid, &g, buffer, len, &g_ptr );
                if ( g_ptr ) {
                    oi.groups.push_back( g.gr_name );
                }
                else {
                    oi.groups.push_back( "" );
                }
            } );
        }

        // Do I need to manually free the memory?
        struct utsname uts_name;
        uname( &uts_name );
        oi.architecture = uts_name.machine;
        oi.os_name      = uts_name.sysname;

        oi.pc_name = c2s_wrapper( []( auto buf, auto len ) { gethostname( buf, len ); } );

        return oi;
    }

}  // namespace lcxx::os::os
