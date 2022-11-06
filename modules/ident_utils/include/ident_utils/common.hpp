#ifndef LCXX__IDENT_UTILS_COMMON_HPP__
#define LCXX__IDENT_UTILS_COMMON_HPP__

#include <concepts>
#include <cstring>
#include <fstream>
#include <sstream>

namespace lcxx::experimental::ident_utils {

    inline auto cat_file( std::string const & file_path ) -> std::string
    {
        std::ifstream     ifs{ file_path };
        std::stringstream ss( "" );
        ss << ifs.rdbuf();
        return ss.str();
    };

    template < std::size_t BufferSize = 1024 >
    inline auto c2s_wrapper( std::invocable< char *, std::size_t > auto f ) -> std::string
    {
        std::string tmp( BufferSize, 0 );
        f( tmp.data(), BufferSize );
        tmp.resize( std::strlen( tmp.c_str() ) );
        return tmp;
    }

}  // namespace lcxx::experimental::ident_utils

#endif  // LCXX__IDENT_UTILS_COMMON_HPP__
