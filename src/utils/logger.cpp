#include <fstream>
#include <iostream>
#include <string>

namespace blink::utils {

void log_message(const std::string& message) {
    std::ofstream log("/tmp/blink.log", std::ios::app);
    if (log.is_open()) {
        log << message << '\n';
    }
    std::cout << message << std::endl;
}

}
