#ifndef LCXX__LCXX_READER_HPP__
#define LCXX__LCXX_READER_HPP__

#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

#include <lcxx/license.hpp>

namespace lcxx {

    auto from_json( nlohmann::json const & license_json ) -> std::pair< license, signature >;
    auto from_json( std::string const & license_str ) -> std::pair< license, signature >;
    auto from_json( std::span< char > const license_str ) -> std::pair< license, signature >;
    auto from_json( std::filesystem::path const & license_path ) -> std::pair< license, signature >;

}  // namespace lcxx

#endif  // LCXX__LCXX_READER_HPP__
