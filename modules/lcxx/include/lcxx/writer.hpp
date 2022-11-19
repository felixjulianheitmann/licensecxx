#ifndef LCXX__LCXX_WRITER_HPP__
#define LCXX__LCXX_WRITER_HPP__

#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

#include <lcxx/license.hpp>
#include <lcxx/rsa.hpp>

namespace lcxx {

    /**
     * @brief generates the output json and signs it using the private key, if provided
     *
     * @param license the license object which will be serialized
     * @param private_key the private key used to create the signature if provided
     * @return nlohmann::json a json object containing the serialized license and the signature
     */
    auto to_json( license const & license, crypto::rsa::key_t const private_key = crypto::rsa::key_t{} )
        -> nlohmann::json;

    /**
     * @brief generates the output json and signs it, if a private key is provided, writing it to the file pointed to by
     * `output_file`
     *
     * @param license the license object which will be serialized
     * @param output_file a system path pointing to the target file - will be overwritten, if it points to an existing
     * @param private_key the private key used to create the signature
     * file
     */
    void to_file( license const & license, std::filesystem::path const & output_file,
                  crypto::rsa::key_t const private_key = crypto::rsa::key_t{} );

    /**
     * @brief generates the output json and signs it, if a private key is provided, serializing it to a string
     *
     * @param license the license object which will be serialized
     * @param private_key the private key used to create the signature
     * @return std::string a serialized string containing all the license info
     */
    auto to_string( license const & license, crypto::rsa::key_t const private_key = crypto::rsa::key_t{} )
        -> std::string;

}  // namespace lcxx

#endif  // LCXX__LCXX_WRITER_HPP__