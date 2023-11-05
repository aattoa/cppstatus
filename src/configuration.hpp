#include "block.hpp"
#include <array>

namespace cppstatus {

    using namespace std::chrono_literals;

    constexpr auto blocks = std::to_array<Block>({
        { .update_interval = 1s, .callback = callbacks::time },
    });

} // namespace cppstatus
