#ifndef LCXX__OS_UTILS_CPU_UTILS_HPP__
#define LCXX__OS_UTILS_CPU_UTILS_HPP__

#include <cstddef>

namespace lcxx::os::cpu {

    struct cpu_info {
        std::size_t n_cores;
        std::size_t max_frequency;
    };

    auto get_info() -> cpu_info;

}  // namespace lcxx::os::cpu

#endif  // LCXX__OS_UTILS_CPU_UTILS_HPP__
