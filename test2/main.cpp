#include "vlk_app.h"
#include <iostream>
#include <stdexcept>

int main() {
    sve::TestApp app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}