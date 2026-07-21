#include <string>

namespace blink::ui {

std::string dialog_message(const std::string& title, const std::string& body) {
    return title + "\n" + body;
}

}
