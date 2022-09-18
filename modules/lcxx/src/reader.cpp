#include <lcxx/reader.hpp>

#include <fmt/core.h>
#include <fstream>

namespace lcxx {

    auto parse_json( nlohmann::json const & license_json ) -> std::pair< license, signature >
    {
        license l;

        if ( !license_json.contains( signature_key ) || !license_json.contains( content_key ) ) {
            throw std::invalid_argument(
                fmt::format( "The input json does not contain they keys {} and {}", signature_key, content_key ) );
        }

        for ( auto const & item : license_json.items() ) {
            l.push_content( item.key(), item.value().get< std::string >() );
        }
        return {};
    }

    auto parse_json( std::string const & license_str ) -> std::pair< license, signature >
    {
        return parse_json( nlohmann::json::parse( license_str.begin(), license_str.end() ) );
    }

    auto parse_json( std::span< char > const license_str ) -> std::pair< license, signature >
    {
        return parse_json( nlohmann::json::parse( license_str.begin(), license_str.end() ) );
    }

    auto parse_json( std::filesystem::path const & license_path ) -> std::pair< license, signature >
    {
        return parse_json(
            nlohmann::json::parse( std::ifstream( std::filesystem::absolute( license_path ).string() ) ) );
    }

}  // namespace lcxx