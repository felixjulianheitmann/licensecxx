#ifndef LCXX__OS_UTILS_CPU_UTILS_HPP__
#define LCXX__OS_UTILS_CPU_UTILS_HPP__

#include <string>

namespace lcxx::os::cpu {

    struct cpu_info {
        std::size_t n_cores;
        std::size_t n_threads;
        std::size_t max_frequency;
        std::string vendor;
        std::string model_name;
    };

    auto get_info() -> cpu_info;

}  // namespace lcxx::os::cpu

#endif  // LCXX__OS_UTILS_CPU_UTILS_HPP__
