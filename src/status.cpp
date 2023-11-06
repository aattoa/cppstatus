#include "status.hpp"
#include <format>

namespace {
    auto format_block(std::string& output, cppstatus::Block const& block)
    {
        if (block.format.empty()) {
            block.callback(std::back_inserter(output), block.argument);
        }
        else {
            static std::string string;
            string.clear();
            block.callback(std::back_inserter(string), block.argument);
            std::vformat_to(
                std::back_inserter(output), block.format, std::make_format_args(string));
        }
    }
} // namespace

auto cppstatus::write_status(std::string& output, Configuration const& configuration) -> void
{
    output.append(configuration.left_padding);

    auto       it  = configuration.blocks.begin();
    auto const end = configuration.blocks.end();

    if (it != end) {
        format_block(output, *it);
        for (++it; it != end; ++it) {
            output.append(configuration.block_delimiter);
            format_block(output, *it);
        }
    }

    output.append(configuration.right_padding);
}
