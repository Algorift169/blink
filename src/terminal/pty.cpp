#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <pty.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <utility>

#include <glib-unix.h>

#include "blink/terminal/pty.hpp"

namespace blink::terminal {

namespace {

void close_fd(int& fd) {
    if (fd >= 0) {
        ::close(fd);
        fd = -1;
    }
}

}  

PtySession::PtySession(OutputHandler output_handler)
    : output_handler_(std::move(output_handler)) {
}

PtySession::~PtySession() {
    close();
}

bool PtySession::start() {
    if (master_fd_ >= 0 || child_pid_ >= 0) {
        return false;
    }

    shell_path_ = resolve_shell_path();
    auto pid = forkpty(&master_fd_, nullptr, nullptr, nullptr);
    if (pid < 0) {
        return false;
    }

    if (pid == 0) {
        setenv("TERM", "xterm-256color", 1);
        setenv("PS1", "\\u@\\h : \\w \\$ ", 1);
        setenv("PS2", "> ", 1);
        setenv("PROMPT", "%n@%m : %~ %# ", 1);
        setenv("LANG", "C.UTF-8", 1);
        setenv("LC_ALL", "C.UTF-8", 1);
        unsetenv("BASH_ENV");
        unsetenv("ENV");
        const char* shell_name = strrchr(shell_path_.c_str(), '/');
        if (shell_name == nullptr) shell_name = shell_path_.c_str();
        else shell_name++;
        execl(shell_path_.c_str(), shell_name, "-i", nullptr);
        _exit(127);
    }

    child_pid_ = pid;
    watch_id_ = g_unix_fd_add(master_fd_, static_cast<GIOCondition>(G_IO_IN | G_IO_ERR | G_IO_HUP), on_master_ready, this);
    return true;
}

void PtySession::close() {
    if (watch_id_ != 0) {
        g_source_remove(watch_id_);
        watch_id_ = 0;
    }

    if (child_pid_ > 0) {
        kill(child_pid_, SIGTERM);
        waitpid(child_pid_, nullptr, WNOHANG);
        child_pid_ = -1;
    }

    close_fd(master_fd_);
}

bool PtySession::write(const char* data, std::size_t size) {
    if (master_fd_ < 0 || data == nullptr || size == 0) {
        return false;
    }

    ssize_t written = ::write(master_fd_, data, size);
    return written == static_cast<ssize_t>(size);
}

void PtySession::resize(int cols, int rows) {
    if (master_fd_ < 0) {
        return;
    }

    struct winsize ws {
        static_cast<unsigned short>(rows), static_cast<unsigned short>(cols), 0, 0
    };
    ioctl(master_fd_, TIOCSWINSZ, &ws);
    if (child_pid_ > 0) {
        kill(child_pid_, SIGWINCH);
    }
}

gboolean PtySession::on_master_ready(gint fd, GIOCondition condition, gpointer data) {
    auto* session = static_cast<PtySession*>(data);
    if (condition & (G_IO_HUP | G_IO_ERR)) {
        session->close();
        return G_SOURCE_REMOVE;
    }

    std::array<char, 4096> buffer{};
    ssize_t count = read(fd, buffer.data(), buffer.size());
    if (count <= 0) {
        session->close();
        return G_SOURCE_REMOVE;
    }

    session->handle_output(buffer.data(), static_cast<std::size_t>(count));
    return G_SOURCE_CONTINUE;
}

void PtySession::handle_output(const char* data, std::size_t size) {
    if (output_handler_) {
        output_handler_(std::string(data, size));
    }
}

}  
