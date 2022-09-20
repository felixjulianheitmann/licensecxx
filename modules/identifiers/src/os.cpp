#include <lcxx/identifiers/os.hpp>

#include <ident_utils/os_utils.hpp>
#include <lcxx/encoding.hpp>
#include <lcxx/hash.hpp>
#include <lcxx/identifiers/common.hpp>
#include <nlohmann/json.hpp>

namespace lcxx::ident_utils::os {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( os_info, os_name, os_architecture, os_pc_name, user_name, user_groups, user_uid,
                                        user_gids );
}

namespace lcxx::identifiers {

    os_ident_strat operator|( os_ident_strat const lhs, os_ident_strat const rhs )
    {
        using base_t = std::underlying_type_t< os_ident_strat >;
        return static_cast< os_ident_strat >( static_cast< base_t >( lhs ) | static_cast< base_t >( rhs ) );
    }

    auto os( os_ident_strat const strategy ) -> identifier
    {
        auto os_info = ident_utils::os::get_info();

        nlohmann::json id_json;

        if ( strat_active( strategy, os_ident_strat::all ) ) {
            auto msg = nlohmann::json{ os_info }.dump();
            return {
                encode::base64( hash::md5( msg ) ),
                msg,
            };
        }

        if ( strat_active( strategy, os_ident_strat::os ) ) {
            id_json["os_name"]         = os_info.os_name;
            id_json["os_architecture"] = os_info.os_architecture;
            id_json["os_pc_name"]      = os_info.os_pc_name;
        }
        else {
            if ( strat_active( strategy, os_ident_strat::os_name ) )
                id_json["os_name"] = os_info.os_name;
            if ( strat_active( strategy, os_ident_strat::os_architecture ) )
                id_json["os_architecture"] = os_info.os_architecture;
            if ( strat_active( strategy, os_ident_strat::os_pc_name ) )
                id_json["os_pc_name"] = os_info.os_pc_name;
        }
        if ( strat_active( strategy, os_ident_strat::user ) ) {
            id_json["user_name"]   = os_info.user_name;
            id_json["user_groups"] = os_info.user_groups;
            id_json["user_gids"]   = os_info.user_gids;
        }
        else {
            if ( strat_active( strategy, os_ident_strat::user_name ) )
                id_json["user_name"] = os_info.user_name;
            if ( strat_active( strategy, os_ident_strat::user_groups ) )
                id_json["user_groups"] = os_info.user_groups;
            if ( strat_active( strategy, os_ident_strat::user_gids ) )
                id_json["user_gids"] = os_info.user_gids;
        }

        auto msg = id_json.dump();
        return { encode::base64( hash::md5( msg ) ), msg };
    }

    auto verify( os_ident_strat const strategy, std::string_view const hash ) -> bool
    {
        return hash == os( strategy ).hash;
    }
}  // namespace lcxx::identifiers
