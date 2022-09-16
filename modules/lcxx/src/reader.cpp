#include <lcxx/reader.hpp>

#include <fstream>

namespace lcxx {

    std::pair< license, signature > parse_json( nlohmann::json const & license_json )
    {
        // TODO PARSE JSON to license
        return {};
    }

    std::pair< license, signature > parse_json( std::string const & license_str )
    {
        return parse_json( nlohmann::json::parse( license_str.begin(), license_str.end() ) );
    }

    std::pair< license, signature > parse_json( std::span< char > const license_str )
    {
        return parse_json( nlohmann::json::parse( license_str.begin(), license_str.end() ) );
    }

    std::pair< license, signature > parse_json( std::filesystem::path const & license_path )
    {
        return parse_json(
            nlohmann::json::parse( std::ifstream( std::filesystem::absolute( license_path ).string() ) ) );
    }

}  // namespace lcxx