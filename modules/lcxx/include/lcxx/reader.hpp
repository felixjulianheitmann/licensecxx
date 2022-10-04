#ifndef LCXX__LCXX_READER_HPP__
#define LCXX__LCXX_READER_HPP__

#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

#include <lcxx/license.hpp>

namespace lcxx {

    /**
     * @brief parse license and license signature from a json object
     *
     * @param license_json the json object containing license and signature
     * @return std::pair< license, signature > license content and signature
     */
    auto from_json( nlohmann::json const & license_json ) -> std::pair< license, signature >;

    /**
     * @brief parse license and license signature from a string serialized json object
     *
     * @param license_str the serialized json string object containing license and signature
     * @return std::pair< license, signature > license content and signature
     */
    auto from_string( std::string const & license_str ) -> std::pair< license, signature >;

    /**
     * @brief parse license and license signature from a string serialized json object
     *
     * @param license_str the json object containing license and signature
     * @return std::pair< license, signature > license content and signature
     */
    auto from_bytes( std::span< char > const license_str ) -> std::pair< license, signature >;

    /**
     * @brief parse license and license signature from a json file
     *
     * @param license_path the path to the json file
     * @return std::pair< license, signature > license content and signature
     */
    auto from_file( std::filesystem::path const & license_path ) -> std::pair< license, signature >;

}  // namespace lcxx

#endif  // LCXX__LCXX_READER_HPP__
