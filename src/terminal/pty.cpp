#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <pty.h>
#include <regex>
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
        setenv("SHELL", shell_path_.c_str(), 1);
        setenv("PS1", "\\[\\e[34m\\]\\u@\\h\\[\\e[0m\\] : \\[\\e[33m\\]\\w\\[\\e[0m\\] \\[\\e[31m\\]$(if [ \"$EUID\" -eq 0 ]; then echo '&'; else echo '\\$'; fi)\\[\\e[0m\\] ", 1);
        setenv("PROMPT_COMMAND", "PS1='\\[\\e[34m\\]\\u@\\h\\[\\e[0m\\] : \\[\\e[33m\\]\\w\\[\\e[0m\\] \\[\\e[31m\\]$(if [ \"$EUID\" -eq 0 ]; then echo '&'; else echo '\\$'; fi)\\[\\e[0m\\] '", 1);
        setenv("PS2", "> ", 1);
        setenv("LANG", "C.UTF-8", 1);
        setenv("LC_ALL", "C.UTF-8", 1);
        unsetenv("BASH_ENV");
        unsetenv("ENV");
        const char* shell_name = strrchr(shell_path_.c_str(), '/');
        if (shell_name == nullptr) shell_name = shell_path_.c_str();
        else shell_name++;
        if (std::strcmp(shell_name, "bash") == 0) {
            execl(shell_path_.c_str(), shell_name, "--noprofile", "--norc", "-i", nullptr);
        } else if (std::strcmp(shell_name, "zsh") == 0) {
            setenv("PS1", "%F{blue}%n@%m%f : %F{yellow}%~%f %(!.%F{red}&.%f:%F{red}$%f) ", 1);
            execl(shell_path_.c_str(), shell_name, "-f", "-i", nullptr);
        } else {
            execl(shell_path_.c_str(), shell_name, "-i", nullptr);
        }
        _exit(127);
    }

    child_pid_ = pid;
    watch_id_ = g_unix_fd_add(master_fd_, static_cast<GIOCondition>(G_IO_IN | G_IO_ERR | G_IO_HUP), on_master_ready, this);
    return true;
}

std::string normalize_prompt_output(const std::string& text) {
    static const std::regex prompt_re(R"((^|\n)(?:\x1B\[[0-9;]*m)*([^@\n]+)@([^:\n]+): ?([^\n#$]+?)([$#]) ?)");
    static const std::string replacement = "$1\033[34m$2@$3\033[0m : \033[33m$4\033[0m \033[31m$5\033[0m ";
    return std::regex_replace(text, prompt_re, replacement);
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
    std::string input(data, size);
    input = normalize_prompt_output(input);
    if (output_handler_) {
        output_handler_(input);
    }
}

}  
