#include <cstddef>
#include <string>
#include <vector>

namespace blink::terminal {

std::vector<std::string> limit_scrollback(const std::vector<std::string>& lines, std::size_t max_lines) {
    if (lines.size() <= max_lines) {
        return lines;
    }
    return std::vector<std::string>(lines.end() - static_cast<std::ptrdiff_t>(max_lines), lines.end());
}

std::string join_scrollback(const std::vector<std::string>& lines) {
    std::string output;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (i != 0) {
            output.push_back('\n');
        }
        output += lines[i];
    }
    return output;
}

}
