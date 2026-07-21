#include <cstdio>
#include <memory>
#include <string>
#include <vector>

namespace blink::utils {

std::string run_command(const std::vector<std::string>& args) {
    std::string command;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (i != 0) {
            command.push_back(' ');
        }
        command += args[i];
    }
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        return {};
    }
    std::string output;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        output += buffer;
    }
    return output;
}

}
