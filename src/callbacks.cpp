#include "status.hpp"

#include <span>
#include <array>
#include <chrono>

// Have to include `stdio.h` instead of `cstdio` for non-standard popen/pclose
#include <stdio.h>

namespace {
    constexpr std::size_t command_output_size = 256;

    auto execute_command(
        std::span<char, command_output_size> const output, char const* const command) -> bool
    {
        auto const  previous_errno = errno;
        FILE* const pipe           = popen(command, "r");
        if (!pipe || errno != previous_errno) {
            return false;
        }
        if (!fgets(output.data(), output.size(), pipe)) {
            return false;
        }
        if (pclose(pipe) == -1 || errno == ECHILD) {
            return false;
        }
        auto const it = std::find(output.begin(), output.end(), '\n');
        if (it != output.end()) {
            *it = '\0';
        }
        return output.front() != '\0';
    }
} // namespace

auto cppstatus::callbacks::date_time(Block::Out const out, char const* const format) -> void
{
    using namespace std::chrono;
    auto const time_now = current_zone()->to_local(system_clock::now());
    std::vformat_to(out, format, std::make_format_args(floor<seconds>(time_now)));
}

auto cppstatus::callbacks::command(Block::Out const out, char const* const command) -> void
{
    std::array<char, 256> command_output;
    command_output.fill(0);
    if (execute_command(command_output, command)) {
        std::format_to(out, "{}", command_output.data());
    }
    else {
        std::format_to(out, "(?)");
    }
}

auto cppstatus::callbacks::file_content(Block::Out out, char const* const path) -> void
{
    FILE* const file = fopen(path, "r");
    if (!file) {
        std::format_to(out, "(?)");
        return;
    }

    for (;;) {
        int const character = fgetc(file);
        if (character == EOF || character == '\n') {
            break;
        }
        *out++ = character;
    }

    fclose(file);
}
