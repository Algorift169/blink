#include <string>

namespace blink::ui {

std::string titlebar_text(const std::string& title, bool modified) {
    return modified ? title + " •" : title;
}

}
