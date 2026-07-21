#include <algorithm>
#include <string>
#include <vector>

namespace blink::clipboard {

namespace {
constexpr std::size_t kMaxHistory = 25;
}

class ClipboardHistoryStore {
public:
    static ClipboardHistoryStore& instance() {
        static ClipboardHistoryStore store;
        return store;
    }

    void push(const std::string& value) {
        if (value.empty()) {
            return;
        }
        entries_.erase(std::remove(entries_.begin(), entries_.end(), value), entries_.end());
        entries_.insert(entries_.begin(), value);
        if (entries_.size() > kMaxHistory) {
            entries_.resize(kMaxHistory);
        }
    }

    std::vector<std::string> recent(std::size_t limit = kMaxHistory) const {
        std::vector<std::string> copy(entries_.begin(), entries_.begin() + std::min(limit, entries_.size()));
        return copy;
    }

private:
    std::vector<std::string> entries_;
};

void add_to_history(const std::string& value) {
    ClipboardHistoryStore::instance().push(value);
}

std::vector<std::string> recent_history(std::size_t limit) {
    return ClipboardHistoryStore::instance().recent(limit);
}

}
