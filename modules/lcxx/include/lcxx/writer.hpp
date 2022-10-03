#ifndef LCXX__LCXX_WRITER_HPP__
#define LCXX__LCXX_WRITER_HPP__

#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

#include <lcxx/license.hpp>
#include <lcxx/rsa.hpp>

namespace lcxx {

    /**
     * @brief Calculates the signature and generates the output json
     *
     * @param license the license object, which will be serialized
     * @param private_key the private key used to create the signature
     * @return nlohmann::json a json object containing the serialized license and the signature
     */
    auto to_json( license const & license, crypto::rsa::key_t const private_key ) -> nlohmann::json;

    /**
     * @brief Calculates the signature and generates the output json, writing it to the file pointed to by `output_file`
     *
     * @param license the license object, which will be serialized
     * @param output_file a system path pointing to the target file - will be overwritten, if it points to an existing
     * @param private_key the private key used to create the signature
     * file
     */
    void to_json( license const & license, std::filesystem::path const & output_file,
                  crypto::rsa::key_t const private_key );

}  // namespace lcxx

#endif  // LCXX__LCXX_WRITER_HPP__