#include <string>
#include <unordered_map>

namespace blink::ui {

class SettingsStore {
public:
    static SettingsStore& instance() {
        static SettingsStore store;
        return store;
    }

    void set(const std::string& key, const std::string& value) {
        values_[key] = value;
    }

    std::string get(const std::string& key, const std::string& fallback = {}) const {
        const auto it = values_.find(key);
        if (it == values_.end()) {
            return fallback;
        }
        return it->second;
    }

private:
    std::unordered_map<std::string, std::string> values_;
};

void save_setting(const std::string& key, const std::string& value) {
    SettingsStore::instance().set(key, value);
}

std::string load_setting(const std::string& key, const std::string& fallback) {
    return SettingsStore::instance().get(key, fallback);
}

}
