#include <cstdlib>
#include <filesystem>
#include <string>

namespace blink::utils {

bool path_exists(const std::string& path) {
    return std::filesystem::exists(path);
}

std::string join_path(const std::string& left, const std::string& right) {
    return std::filesystem::path(left) / right;
}

std::string home_directory() {
    const char* home = std::getenv("HOME");
    return home != nullptr ? home : "/home/blink";
}

}
