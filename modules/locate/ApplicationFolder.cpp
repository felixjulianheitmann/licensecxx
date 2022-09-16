/*
 * ApplicationFolder.cpp
 *
 *  Created on: Oct 12, 2019
 *      Author: Gabriele Contini
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <lcxx/datatypes.h>
#include <lcxx/properties.h>

#include "ApplicationFolder.hpp"
#include "base/EventRegistry.h"
#include "base/base.h"
#include "base/file_utils.hpp"
#include "base/logger.h"
#include "os/os.h"

namespace license { namespace locate {
    using namespace std;

    ApplicationFolder::ApplicationFolder() : LocatorStrategy( "ApplicationFolder" ) {}

    ApplicationFolder::~ApplicationFolder() {}

    const vector< string > ApplicationFolder::license_locations( EventRegistry & eventRegistry )
    {
        vector< string >      diskFiles;
        char                  fname[MAX_PATH] = { 0 };
        const FUNCTION_RETURN fret            = getModuleName( fname );
        if ( fret == FUNC_RET_OK ) {
            const string module_name       = remove_extension( fname );
            const string temptativeLicense = string( module_name ) + LCC_LICENSE_FILE_EXTENSION;
            ifstream     f( temptativeLicense.c_str() );
            if ( f.good() ) {
                diskFiles.push_back( temptativeLicense );
                eventRegistry.addEvent( LICENSE_FOUND, temptativeLicense.c_str() );
            }
            else {
                eventRegistry.addEvent( LICENSE_FILE_NOT_FOUND, temptativeLicense.c_str() );
            }
            f.close();
        }
        else {
            LOG_WARN( "Error determining module name." );
        }
        return diskFiles;
    }

}}  // namespace license::locate
