#ifndef LCXX__IDENT_UTILS_COMMON_MAC_HPP__
#define LCXX__IDENT_UTILS_COMMON_MAC_HPP__

#if not defined( MAC )
#error "This header should only be included on mac platforms
#endif

#include <cstring>
#include <string>

#include <sys/sysctl.h>

inline auto sysctl_base( std::string const & key, auto & buf, auto & buf_size )
{
    if ( sysctlbyname( key.c_str(), &buf, &size, nullptr, 0 ) < 0 ) {
        buf = {};
    }
    return buf;
};
inline auto sysctl_str( std::string const & key )
{
    std::string buf( 1024, 0 );
    auto        size = buf.size();
    buf              = sysctl_base( key, buf, size );
    buf.resize( std::strlen, 0 );
    return buf;
};
inline auto sysctl_num( std::string const & key )
{
    std::size_t buf;
    auto        size = sizeof( buf );
    return sysctl_base( key, buf, size );
};

#endif  // LCXX__IDENT_UTILS_COMMON_MAC_HPP__
