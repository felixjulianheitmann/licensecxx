#include <ident_utils/os_utils.hpp>

#include <algorithm>
#include <grp.h>
#include <ranges>
#include <sys/unistd.h>
#include <sys/utsname.h>

#include <ident_utils/common.hpp>

namespace lcxx::ident_utils::os {

    auto get_info() -> os_info
    {
        os_info oi;

        oi.user_name = c2s_wrapper( []( auto buffer, auto len ) { getlogin_r( buffer, len ); } );
        oi.user_uid  = getuid();

        int                  buffer_len = 64;
        std::vector< gid_t > group_ids( buffer_len, 0 );
        getgrouplist( oi.user_name.c_str(), getgid(), group_ids.data(), &buffer_len );
        std::transform( group_ids.begin(), group_ids.begin() + buffer_len, std::back_inserter( oi.user_gids ),
                        []( auto const id ) -> std::size_t { return id; } );
        std::ranges::sort( oi.user_gids );

        for ( auto const gid : oi.user_gids ) {
            c2s_wrapper( [&]( auto buffer, auto len ) {
                struct group   g;
                struct group * g_ptr;
                getgrgid_r( gid, &g, buffer, len, &g_ptr );
                if ( g_ptr ) {
                    oi.user_groups.push_back( g.gr_name );
                }
                else {
                    oi.user_groups.push_back( "" );
                }
            } );
        }

        // Do I need to manually free the memory?
        struct utsname uts_name;
        uname( &uts_name );
        oi.os_architecture = uts_name.machine;
        oi.os_name         = uts_name.sysname;

        oi.os_pc_name = c2s_wrapper( []( auto buf, auto len ) { gethostname( buf, len ); } );

        return oi;
    }

}  // namespace lcxx::ident_utils::os
