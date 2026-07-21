#include <fstream>
#include <string>
#include <vector>

namespace blink::clipboard {

namespace {
constexpr const char* kClipboardStore = "/tmp/blink_clipboard.txt";
}

std::string read_text() {
    std::ifstream input(kClipboardStore);
    if (!input.is_open()) {
        return {};
    }
    std::string content((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    return content;
}

bool write_text(const std::string& text) {
    std::ofstream output(kClipboardStore, std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }
    output << text;
    return true;
}

bool clear() {
    return write_text("");
}

std::vector<std::string> history_entries() {
    const std::string current = read_text();
    return current.empty() ? std::vector<std::string>{} : std::vector<std::string>{current};
}

}
