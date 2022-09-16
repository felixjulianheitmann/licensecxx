#define BOOST_TEST_MODULE dmi_info_test
#include "../../../src/library/os/dmi_info.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

namespace license { namespace test {
    using namespace std;

    BOOST_AUTO_TEST_CASE( dmi_info )
    {
        os::DmiInfo dmiInfo;

        BOOST_CHECK_MESSAGE( dmiInfo.bios_vendor().size() > 0, "Bios vendor length >0" );
        BOOST_CHECK_MESSAGE( dmiInfo.bios_description().size() > 0, "Bios description length >0" );
        BOOST_CHECK_MESSAGE( dmiInfo.sys_vendor().size() > 0, "Sys vendor length >0" );
    }

}}  // namespace license::test
