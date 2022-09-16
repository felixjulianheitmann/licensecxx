#ifndef LCXX__LCXX_DATATYPES_HPP__
#define LCXX__LCXX_DATATYPES_HPP__

#include <string>
#include <vector>

#ifdef __unix__
#define DllExport
#ifndef MAX_PATH
#define MAX_PATH 1024
#endif
#else
#include <windows.h>
#define DllExport __declspec( dllexport )
#endif

namespace lcxx {

    enum class hw_identification_strategy {
        /**
         * \brief Default strategy.
         *
         * This strategy first checks the content of the environment variable `IDENTIFICATION_STRATEGY`. If the variable
         * is defined it will use the strategy specified in there, if not defined it will try to detect which virtual
         * environment the software is running in.
         *  - If no virtual environment is detected it will try the strategies defined in ::BARE_TO_METAL_STRATEGIES
         *  - If it detects the software is running in a virtual machine it will use ::VM_STRATEGIES
         *  - If it detects the software is running in a docker or in an LXC it will use ::DOCKER_STRATEGIES or
         * ::DOCKER_STRATEGIES
         *  - If it detects the software is running in a virtual machine in the cloud it will use ::CLOUD_STRATEGIES
         */
        STRATEGY_DEFAULT = -1,
        /**
         * \brief Uses mac address to generate hardware id.
         *
         * This strategy use the mac address of one of the network adapters to generate the hardware identifier.
         * Network adapters are scored using their names, trying to avoid vpn adapters and virtual devices.
         * Mac address should be unique in the subnetwork, so this strategy poses some limitation on the number of
         * licenses that can be in use in the same subnet.
         */
        STRATEGY_ETHERNET   = 0,
        STRATEGY_IP_ADDRESS = 1,
        STRATEGY_DISK       = 2,
        /**
         * Not yet implemented
         */
        STRATEGY_CPU_SIZE = 3,
        /**
         * Not yet implemented
         */
        STRATEGY_HOST_NAME = 4,
        STRATEGY_NONE      = -2
    };

    enum class event_type {
        LICENSE_OK                       = 0,  // OK
        LICENSE_FILE_NOT_FOUND           = 1,  // license file not found
        LICENSE_SERVER_NOT_FOUND         = 2,  // license server can't be contacted
        ENVIRONMENT_VARIABLE_NOT_DEFINED = 3,  // environment variable not defined
        FILE_FORMAT_NOT_RECOGNIZED       = 4,  // license file has invalid format (not .ini file)
        LICENSE_MALFORMED                = 5,  // some mandatory field are missing, or data can't be fully read.
        PRODUCT_NOT_LICENSED             = 6,  // this product was not licensed
        PRODUCT_EXPIRED                  = 7,  //!< PRODUCT_EXPIRED
        LICENSE_CORRUPTED                = 8,  // License signature didn't match with current license
        IDENTIFIERS_MISMATCH             = 9,  // Calculated identifier and the one provided in license didn't match

        LICENSE_SPECIFIED  = 100,  // license location was specified
        LICENSE_FOUND      = 101,  // License file has been found or license data has been located
        PRODUCT_FOUND      = 102,  // License has been loaded and the declared product has been found
        SIGNATURE_VERIFIED = 103   //!< SIGNATURE_VERIFIED
    };

    enum class license_type {
        LOCAL,
        REMOTE  // remote licenses are not supported now.
    };

    enum class severity { SVRT_INFO, SVRT_WARN, SVRT_ERROR };

    struct audit_event {
        severity   severity;
        event_type event_type;
        /**
         * License file name or location where the license is stored.
         */
        std::string license_reference;
        // TODO
        std::string param2;
    };

    enum class license_data_type {
        /**
         * A list of absolute path separated by ';' containing the eventual location
         * of the license files. Can be NULL.
         */
        LICENSE_PATH,
        /**
         * The license is provided as plain data
         */
        LICENSE_PLAIN_DATA,
        /**
         * The license is encoded
         */
        LICENSE_ENCODED
    };

    /**
     * This structure contains informations on the raw license data. Software authors
     * can specify the location of the license file or its full content.
     *
     * Can be NULL, in this case OpenLicenseManager will try to figure out the
     * license file location on its own.
     */
    struct license_location {
        license_data_type license_data_type;
        std::string       license_data;
    };

    /**
     * Informations about the software requesting the license verification (eg, software version, feature to verify).
     */
    struct caller_information {
        /**
         *  software version in format xxxx[.xxxx.xxxx]
         *  NOT IMPLEMENTED pass '\0'
         */
        std::string version;
        /**
         * Name of the feature you want to verify. If empty ('\0') the 'default' feature will be verified.
         * (every project has a default feature that is equal to the project name).
         * Every feature has a separate section in the license file:
         * <pre>
         * [feature_xx]
         * sig=AAAA
         * [another_feature]
         * expiry-date=20201111
         * </pre>
         */
        std::string feature_name;
        /**
         * this number passed in by the application must correspond to the magic number used when compiling the library.
         * See cmake parameter -DPROJECT_MAGIC_NUM and licensecc_properties.h macro VERIFY_MAGIC
         */
        unsigned int magic;
    };

    struct license_info {
        /**
         * Detailed reason of success/failure. Reasons for a failure can be
         * multiple (for instance, license expired and signature not verified).
         * Only the last AUDIT_EVENT_NUM are reported.
         */
        std::vector< audit_event > status;
        /**
         * Eventual expiration date of the software,
         * can be '\0' if the software don't expire
         * */
        std::string  expiry_date;
        unsigned int days_left;
        bool         has_expiry;
        bool         linked_to_pc;
        license_type license_type;  // Local or Remote
        /* A string of character inserted into the license understood
         * by the calling application.
         * '\0' if the application didn't specify one */
        std::string proprietary_data;
        int         license_version;  // license file version
    };

    enum class api_virtualization_detail { BARE_TO_METAL, VMWARE, VIRTUALBOX, V_XEN, KVM, HV, PARALLELS, V_OTHER };

    enum class api_cloud_provider {
        PROV_UNKNOWN = 0,
        ON_PREMISE   = 1,
        GOOGLE_CLOUD = 2,
        AZURE_CLOUD  = 3,
        AWS          = 4,
        /**
         * "/sys/class/dmi/id/bios_vendor" SeaBIOS
         * "/sys/class/dmi/id/sys_vendor" Alibaba Cloud
         * modalias
         * "dmi:bvnSeaBIOS:bvrrel-1.7.5-0-ge51488c-20140602_164612-nilsson.home.kraxel.org:bd04/01/2014:svnAlibabaCloud:pnAlibabaCloudECS:pvrpc-i440fx-2.1:cvnAlibabaCloud:ct1:cvrpc-i440fx-2.1:"
         */
        ALI_CLOUD = 5
    };

    enum class api_virtualization_summary { NONE, CONTAINER, VM };

    struct execution_environment_information {
        api_cloud_provider         cloud_provider;
        api_virtualization_summary virtualization;
        api_virtualization_detail  virtualization_detail;
    };

}  // namespace lcxx

#endif
