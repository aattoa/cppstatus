#include "block.hpp"

namespace {
    auto time_now()
    {
        return std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
    }
} // namespace

auto cppstatus::callbacks::time(Block::Out const out, char const*) -> void
{
    std::format_to(out, "{:%T}", std::chrono::floor<std::chrono::seconds>(time_now()));
}
