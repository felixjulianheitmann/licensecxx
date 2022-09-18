#include <lcxx/encoding.hpp>
#include <lcxx/reader.hpp>

#include <algorithm>
#include <fstream>

namespace lcxx {

    auto from_json( nlohmann::json const & license_json ) -> std::pair< license, signature >
    {
        license l;

        if ( !license_json.contains( signature_key ) || !license_json.contains( content_key ) ) {
            throw std::invalid_argument( std::string{ "The input json does not contain they keys " } + signature_key +
                                         " and " + content_key );
        }

        auto const & content = license_json[content_key];
        for ( auto const & item : content.items() ) {
            l.push_content( item.key(), item.value().get< std::string >() );
        }
        return { l, lcxx::decode::base64( license_json[signature_key].get< std::string >() ) };
    }

    auto from_json( std::string const & license_str ) -> std::pair< license, signature >
    {
        return from_json( nlohmann::json::parse( license_str.begin(), license_str.end() ) );
    }

    auto from_json( std::span< char > const license_str ) -> std::pair< license, signature >
    {
        return from_json( nlohmann::json::parse( license_str.begin(), license_str.end() ) );
    }

    auto from_json( std::filesystem::path const & license_path ) -> std::pair< license, signature >
    {
        return from_json(
            nlohmann::json::parse( std::ifstream( std::filesystem::absolute( license_path ).string() ) ) );
    }

}  // namespace lcxx