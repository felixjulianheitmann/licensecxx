#ifndef RETRIEVE_FACTORY_H_
#define RETRIEVE_FACTORY_H_

#include <cstddef>
#include <lcxx/datatypes.h>
#include <lcxx/properties.h>
#include <string>
#include <vector>

#include "LocatorStrategy.hpp"
#include "base/base.h"

namespace license { namespace locate {

    class LocatorFactory {
    private:
        static bool find_license_near_moduleb;
        static bool find_license_with_env_varb;
        inline LocatorFactory() {}
        inline ~LocatorFactory() {}

    public:
        static FUNCTION_RETURN get_active_strategies( std::vector< std::unique_ptr< LocatorStrategy > > & strategiesOut,
                                                      const LicenseLocation * locationHint );
        static void            find_license_near_module( bool enable ) { find_license_near_moduleb = enable; }
        static void            find_license_with_env_var( bool enable ) { find_license_with_env_varb = enable; }
    };

}}  // namespace license::locate

#endif
