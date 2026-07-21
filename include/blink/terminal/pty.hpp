#pragma once

#include <functional>
#include <string>

#include <glib.h>

namespace blink::terminal {

class PtySession {
public:
    using OutputHandler = std::function<void(const std::string&)>
;

    explicit PtySession(OutputHandler output_handler);
    ~PtySession();

    PtySession(const PtySession&) = delete;
    PtySession& operator=(const PtySession&) = delete;

    bool start();
    void close();
    bool write(const char* data, std::size_t size);
    void resize(int cols, int rows);

private:
    static gboolean on_master_ready(gint fd, GIOCondition condition, gpointer data);

    void handle_output(const char* data, std::size_t size);

    OutputHandler output_handler_;
    std::string shell_path_;
    int master_fd_{-1};
    int child_pid_{-1};
    guint watch_id_{0};
};

std::string resolve_shell_path();

}  
