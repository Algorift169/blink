#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

namespace blink::utils {

std::string trim(const std::string& input) {
    std::string copy = input;
    const auto begin = std::find_if(copy.begin(), copy.end(), [](unsigned char ch) { return !std::isspace(ch); });
    const auto end = std::find_if(copy.rbegin(), copy.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base();
    copy.erase(end, copy.end());
    copy.erase(copy.begin(), begin);
    return copy;
}

std::vector<std::string> split(const std::string& input, char delimiter) {
    std::vector<std::string> items;
    std::stringstream stream(input);
    std::string item;
    while (std::getline(stream, item, delimiter)) {
        items.push_back(item);
    }
    return items;
}

std::string to_lower(const std::string& input) {
    std::string output = input;
    std::transform(output.begin(), output.end(), output.begin(), [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return output;
}

}
