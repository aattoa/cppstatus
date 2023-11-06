#include "status.hpp"

#include <array>
#include <thread>
#include <cstdio>
#include <chrono>
#include <memory>

#include <X11/Xlib.h>

namespace chrono = std::chrono;

namespace {
    constexpr cppstatus::Block blocks[] {
        cppstatus::Block {
            .callback = cppstatus::callbacks::command,
            .argument = "amixer get Master | tail -1 | awk -F \"[][]\" '/%/ { print $2 \" \" $4 }'",
            .format   = "Vol {}",
        },
        cppstatus::Block {
            .callback = cppstatus::callbacks::file_content,
            .argument = "/sys/class/power_supply/BAT0/capacity",
            .format   = "Bat {}%",
        },
        cppstatus::Block {
            .callback = cppstatus::callbacks::date_time,
            .argument = "{:%a %d %b, %T}",
        },
    };

    constexpr cppstatus::Configuration configuration {
        .blocks          = blocks,
        .block_delimiter = ", ",
    };

    constexpr auto refresh_interval = chrono::seconds(1);

} // namespace

auto main() -> int
{
    std::unique_ptr<Display, int (*)(Display*)> const display(XOpenDisplay(nullptr), XCloseDisplay);
    Window const                                      window = DefaultRootWindow(display.get());

    std::string title;
    title.reserve(128);

    for (;;) {
        auto const begin_time = std::chrono::system_clock::now();

        // Format the new status bar
        title.clear();
        cppstatus::write_status(title, configuration);

        // Update the title
        XStoreName(display.get(), window, title.c_str());
        XFlush(display.get());

        auto const time_taken = std::chrono::system_clock::now() - begin_time;
        if (time_taken < refresh_interval) {
            std::this_thread::sleep_for(refresh_interval - time_taken);
        }
    }
}
