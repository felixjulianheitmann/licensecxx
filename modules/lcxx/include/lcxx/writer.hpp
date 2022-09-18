#ifndef LCXX__LCXX_WRITER_HPP__
#define LCXX__LCXX_WRITER_HPP__

#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

#include <lcxx/crypto.hpp>
#include <lcxx/license.hpp>

namespace lcxx {

    /**
     * @brief Calculates the signature and generates the output json
     *
     * @param license the license object, which will be serialized
     * @return nlohmann::json a json object containing the serialized license and the signature
     */
    auto to_json( license const & license, crypto::rsa_key_t const private_key ) -> nlohmann::json;

    /**
     * @brief Calculates the signature and generates the output json, writing it to the file pointed to by `output_file`
     *
     * @param license the license object, which will be serialized
     * @param output_file a system path pointing to the target file - will be overwritten, if it points to an existing
     * file
     */
    void to_json( license const & license, std::filesystem::path const & output_file );

}  // namespace lcxx

#endif  // LCXX__LCXX_WRITER_HPP__