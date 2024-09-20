#include <lcxx/identifiers/hardware.hpp>

#include <nlohmann/json.hpp>

#include <ident_utils/cpu_utils.hpp>
#include <lcxx/encoding.hpp>
#include <lcxx/hash.hpp>
#include <lcxx/identifiers/common.hpp>

namespace lcxx::experimental::ident_utils::cpu {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( cpu_info, vendor, model_name, n_cores, n_threads, max_frequency );
}
namespace lcxx::experimental::identifiers {

    auto operator|( hw_ident_strat const lhs, hw_ident_strat const rhs ) -> hw_ident_strat
    {
        using base_t = std::underlying_type_t< hw_ident_strat >;
        return static_cast< hw_ident_strat >( static_cast< base_t >( lhs ) | static_cast< base_t >( rhs ) );
    }

    auto hardware( hw_ident_strat const strategy ) -> identifier
    {
        auto cpu_info = ident_utils::cpu::get_info();

        nlohmann::json info_json;

        if ( strat_active( strategy, hw_ident_strat::all ) ) {
            auto msg = nlohmann::json{ cpu_info }.dump();
            auto [hash, err] = hash::sha512( msg );
            if ( err != hash::error::ok )
                return { error::hash_error, {}, msg };
            return { error::ok, encode::base64( hash ), msg };
        }

        if ( strat_active( strategy, hw_ident_strat::cpu ) ) {
            info_json["cpu"] = { cpu_info };
        }
        else {
            if ( strat_active( strategy, hw_ident_strat::cpu_n_cores ) )
                info_json["n_cores"] = cpu_info.n_cores;
            if ( strat_active( strategy, hw_ident_strat::cpu_n_threads ) )
                info_json["n_threads"] = cpu_info.n_threads;
            if ( strat_active( strategy, hw_ident_strat::cpu_max_frequency ) )
                info_json["max_frequency"] = cpu_info.max_frequency;
            if ( strat_active( strategy, hw_ident_strat::cpu_vendor ) )
                info_json["vendor"] = cpu_info.vendor;
            if ( strat_active( strategy, hw_ident_strat::cpu_model_name ) )
                info_json["model_name"] = cpu_info.model_name;
        }

        auto msg = info_json.dump();
        auto [hash, err] = hash::sha512( msg );
        if ( err != hash::error::ok )
            return { error::hash_error, {}, msg };
        return { error::ok, encode::base64( hash ), msg };
        // if(strat_active(strategy, hw_ident_strat::gpu) {}
        // if(strat_active(strategy, hw_ident_strat::network) {}
    }

    auto verify( hw_ident_strat const strategy, std::string_view const hash ) -> bool
    {
        return hash == hardware( strategy ).hash;
    }

}  // namespace lcxx::experimental::identifiers
