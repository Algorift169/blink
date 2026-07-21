#include <regex>
#include <string>
#include <vector>

namespace blink::search {

bool matches_regex(const std::string& text, const std::string& pattern) {
    try {
        const std::regex regex(pattern);
        return std::regex_search(text, regex);
    } catch (const std::regex_error&) {
        return false;
    }
}

std::vector<std::string> find_matches(const std::string& text, const std::string& pattern) {
    std::vector<std::string> matches;
    try {
        const std::regex regex(pattern);
        std::sregex_iterator begin(text.begin(), text.end(), regex);
        std::sregex_iterator end;
        for (auto it = begin; it != end; ++it) {
            matches.push_back(it->str());
        }
    } catch (const std::regex_error&) {
    }
    return matches;
}

}
