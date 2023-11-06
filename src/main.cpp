#include "status.hpp"

#include <chrono>
#include <memory>

#include <cstdio>
#include <cstdlib>
#include <csignal>

#include <unistd.h>
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

    volatile std::sig_atomic_t keep_running = 1;
} // namespace

auto main() -> int
{
    // Gracefully handle termination requests
    std::signal(SIGTERM, [](int) { keep_running = 0; });
    std::signal(SIGINT, [](int) { keep_running = 0; });

    // Nominally handle SIGUSR1 so it will interrupt usleep
    std::signal(SIGUSR1, [](int) {});

    std::unique_ptr<Display, decltype(XCloseDisplay)*> const display(
        XOpenDisplay(nullptr), XCloseDisplay);

    if (!display) {
        std::fprintf(stderr, "Could not open X display\n");
        return EXIT_FAILURE;
    }

    Window const window = DefaultRootWindow(display.get());

    std::string title;
    title.reserve(128);

    while (keep_running) {
        auto const begin_time = chrono::system_clock::now();

        // Format the new status bar
        title.clear();
        cppstatus::write_status(title, configuration);

        // Update the title
        XStoreName(display.get(), window, title.c_str());
        XFlush(display.get());

        auto const time_taken = chrono::system_clock::now() - begin_time;
        if (time_taken >= refresh_interval) [[unlikely]] {
            continue;
        }

        // Wait until the status bar should be refreshed or until a signal is handled
        auto const wait_time    = refresh_interval - time_taken;
        auto const microseconds = chrono::duration_cast<chrono::microseconds>(wait_time);
        usleep(microseconds.count());
    }
}
