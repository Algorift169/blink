#include <string>
#include <vector>

namespace blink::ui {

std::string tab_title(const std::string& base, int index) {
    return base + " " + std::to_string(index);
}

std::vector<std::string> tab_titles(const std::vector<std::string>& names) {
    std::vector<std::string> titles;
    for (std::size_t i = 0; i < names.size(); ++i) {
        titles.push_back(tab_title(names[i], static_cast<int>(i + 1)));
    }
    return titles;
}

}
