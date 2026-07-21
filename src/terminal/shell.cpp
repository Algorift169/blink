#include <cstdlib>
#include <string>

#include "blink/terminal/pty.hpp"

namespace blink::terminal {

std::string resolve_shell_path() {
    const char* shell = std::getenv("SHELL");
    if (shell != nullptr && shell[0] != '\0') {
        return shell;
    }
    return "/bin/bash";
}

}  
