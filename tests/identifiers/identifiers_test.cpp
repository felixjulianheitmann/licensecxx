#include <gtest/gtest.h>

#include <lcxx/identifiers/hardware.hpp>
#include <lcxx/identifiers/os.hpp>

using namespace lcxx::experimental::identifiers;
using hws = hw_ident_strat;
using oss = os_ident_strat;

template < typename Strat_Enum >
requires( std::is_enum_v< Strat_Enum > ) void full_ident_test( Strat_Enum strat )
{
    std::string hash;
    if constexpr ( std::is_same< hw_ident_strat, Strat_Enum >() ) {
        hash = hardware( strat ).hash;
    }
    else if constexpr ( std::is_same< os_ident_strat, Strat_Enum >() ) {
        hash = os( strat ).hash;
    }

    EXPECT_TRUE( verify( strat, hash ) );
}

TEST( Identifier_Tests, Hardware_Ident_All ) { full_ident_test( hws::all ); }
TEST( Identifier_Tests, Hardware_Ident_cpu ) { full_ident_test( hws::cpu ); }
TEST( Identifier_Tests, Hardware_Ident_cpu_n_cores ) { full_ident_test( hws::cpu_n_cores ); }
TEST( Identifier_Tests, Hardware_Ident_cpu_n_threads ) { full_ident_test( hws::cpu_n_threads ); }
TEST( Identifier_Tests, Hardware_Ident_cpu_max_frequency ) { full_ident_test( hws::cpu_max_frequency ); }
TEST( Identifier_Tests, Hardware_Ident_cpu_vendor ) { full_ident_test( hws::cpu_vendor ); }
TEST( Identifier_Tests, Hardware_Ident_cpu_model_name ) { full_ident_test( hws::cpu_model_name ); }

TEST( Identifier_Tests, Hardware_Ident_Combined0 ) { full_ident_test( hws::cpu_vendor | hws::cpu_max_frequency ); }
TEST( Identifier_Tests, Hardware_Ident_Combined1 )
{
    full_ident_test( hws::cpu_n_cores | hws::cpu_n_threads | hws::cpu_vendor );
}

TEST( Identifier_Tests, OS_Ident_All ) { full_ident_test( oss::all ); }
TEST( Identifier_Tests, OS_Ident_os ) { full_ident_test( oss::os ); }
TEST( Identifier_Tests, OS_Ident_os_name ) { full_ident_test( oss::os_name ); }
TEST( Identifier_Tests, OS_Ident_os_architecture ) { full_ident_test( oss::os_architecture ); }
TEST( Identifier_Tests, OS_Ident_os_pc_name ) { full_ident_test( oss::os_pc_name ); }
TEST( Identifier_Tests, OS_Ident_user ) { full_ident_test( oss::user ); }
TEST( Identifier_Tests, OS_Ident_user_name ) { full_ident_test( oss::user_name ); }
TEST( Identifier_Tests, OS_Ident_user_uid ) { full_ident_test( oss::user_uid ); }
TEST( Identifier_Tests, OS_Ident_user_groups ) { full_ident_test( oss::user_groups ); }
TEST( Identifier_Tests, OS_Ident_user_gids ) { full_ident_test( oss::user_gids ); }

TEST( Identifier_Tests, OS_Ident_Combined0 ) { full_ident_test( oss::os_pc_name | oss::user_uid | oss::user_gids ); }
TEST( Identifier_Tests, OS_Ident_Combined1 ) { full_ident_test( oss::os | oss::user_uid | oss::user_groups ); }
TEST( Identifier_Tests, OS_Ident_Combined2 ) { full_ident_test( oss::os_name | oss::os_pc_name | oss::user ); }