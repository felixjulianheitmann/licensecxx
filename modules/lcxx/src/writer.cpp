#include <lcxx/writer.hpp>

#include <fstream>

#include <lcxx/encoding.hpp>
#include <lcxx/verifier.hpp>

namespace lcxx {

    nlohmann::json to_json( license const & license, crypto::rsa_key_t const private_key )
    {
        auto const     signature = lcxx::sign( license, private_key );
        nlohmann::json lic_json  = {
             { signature_key, encode::base64( signature ) },
             { content_key, {} },
        };

        auto & content = lic_json[content_key];
        for ( auto const & [k, v] : license.get_content() ) {
            content[k] = v;
        }

        return lic_json;
    }

    void to_json( license const & license, std::filesystem::path const & output_file,
                  crypto::rsa_key_t const private_key )
    {
        namespace fs = std::filesystem;
        if ( !output_file.has_filename() ) {
            throw std::invalid_argument( std::string{ "the given output file must point to a file location: " } +
                                         fs::absolute( output_file ).string() );
        }

        auto lic_json = to_json( license, private_key );

        std::ofstream ofs{ fs::absolute( output_file ).string() };
        ofs << lic_json.dump() << std::endl;
    }
}  // namespace lcxx
