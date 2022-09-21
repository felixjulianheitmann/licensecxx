#ifndef LCXX__IDENT_UTILS_CPU_UTILS_HPP__
#define LCXX__IDENT_UTILS_CPU_UTILS_HPP__

#include <string>

namespace lcxx::ident_utils::cpu {

    struct cpu_info {
        std::size_t n_cores;
        std::size_t n_threads;
        std::size_t max_frequency;
        std::string vendor;
        std::string model_name;
    };

    /**
     * @brief retrieves information about the CPU built into this machine
     *
     * @return cpu_info an information object about the CPU
     */
    auto get_info() -> cpu_info;

}  // namespace lcxx::ident_utils::cpu

#endif  // LCXX__IDENT_UTILS_CPU_UTILS_HPP__
