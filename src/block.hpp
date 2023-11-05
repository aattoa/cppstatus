#include <string>
#include <chrono>

namespace cppstatus {
    struct Block {
        using Out      = std::back_insert_iterator<std::string>;
        using Callback = auto(Out, char const*) -> void;
        std::chrono::milliseconds update_interval {};
        Callback*                 callback {};
        char const*               argument {};
    };
} // namespace cppstatus

namespace cppstatus::callbacks {
    Block::Callback time;
}
