#include <cstdlib>
#include <iostream>
#include <exception>

auto main() -> int
try {
    return EXIT_SUCCESS;
}
catch (std::exception const& exception) {
    std::cerr << "Error: " << exception.what() << '\n';
    return EXIT_FAILURE;
}
