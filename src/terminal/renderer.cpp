#include <string>

namespace blink::terminal {

std::string render_status_line(const std::string& title, int cols, int rows) {
    return title + " [" + std::to_string(cols) + "x" + std::to_string(rows) + "]";
}

std::string render_header(const std::string& title) {
    return "Blink :: " + title;
}

}
