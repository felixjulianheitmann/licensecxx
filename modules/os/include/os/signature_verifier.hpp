/*
 * verifier.hpp
 *
 *  Created on: Nov 16, 2019
 *      Author: GC
 */

#ifndef SRC_LIBRARY_OS_VERIFIER_HPP_
#define SRC_LIBRARY_OS_VERIFIER_HPP_

#include "base/base.h"
#include <string>

namespace license { namespace os {
    FUNCTION_RETURN verify_signature( const std::string & stringToVerify, const std::string & signatureB64 );
}}  // namespace license::os

#endif /* SRC_LIBRARY_OS_VERIFIER_HPP_ */
