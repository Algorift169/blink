#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

namespace blink::search {

namespace {
std::string normalize(const std::string& input) {
    std::string result;
    result.reserve(input.size());
    for (unsigned char ch : input) {
        result.push_back(static_cast<char>(std::tolower(ch)));
    }
    return result;
}
}

std::vector<std::string> search_lines(const std::vector<std::string>& lines, const std::string& query) {
    const std::string normalized_query = normalize(query);
    std::vector<std::string> matches;
    for (const auto& line : lines) {
        if (normalize(line).find(normalized_query) != std::string::npos) {
            matches.push_back(line);
        }
    }
    return matches;
}

std::vector<std::string> search_text(const std::string& text, const std::string& query) {
    std::vector<std::string> lines;
    std::string current;
    for (char ch : text) {
        if (ch == '\n') {
            lines.push_back(current);
            current.clear();
        } else {
            current.push_back(ch);
        }
    }
    if (!current.empty()) {
        lines.push_back(current);
    }
    return search_lines(lines, query);
}

}
