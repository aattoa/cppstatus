#include "status.hpp"
#include <format>

auto cppstatus::write_status(std::string& output, Configuration const& configuration) -> void
{
    output.append(configuration.left_padding);

    auto       it  = configuration.blocks.begin();
    auto const end = configuration.blocks.end();

    auto const format_block = [&] { it->callback(std::back_inserter(output), it->argument); };

    if (it != end) {
        format_block();
        for (++it; it != end; ++it) {
            output.append(configuration.block_delimiter);
            format_block();
        }
    }

    output.append(configuration.right_padding);
}
