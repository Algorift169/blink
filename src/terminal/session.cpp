#include <string>

namespace blink::terminal {

std::string normalize_session_name(const std::string& name) {
    if (name.empty()) {
        return "shell";
    }
    return name;
}

bool has_active_session(const std::string& name) {
    return !name.empty();
}

}
