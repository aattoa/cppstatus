#include <span>
#include <string>
#include <iterator>
#include <string_view>

namespace cppstatus {

    struct Block {
        using Out      = std::back_insert_iterator<std::string>;
        using Callback = auto(Out, char const*) -> void;
        Callback*        callback {};
        char const*      argument {};
        std::string_view format;
    };

    struct Configuration {
        std::span<Block const> blocks;
        std::string_view       block_delimiter;
        std::string_view       left_padding;
        std::string_view       right_padding;
    };

    auto write_status(std::string&, Configuration const&) -> void;

} // namespace cppstatus

namespace cppstatus::callbacks {
    Block::Callback date_time;
    Block::Callback command;
    Block::Callback file_content;
} // namespace cppstatus::callbacks
