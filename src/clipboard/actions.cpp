#include "blink/clipboard/actions.hpp"

namespace blink::clipboard {

void copy_selection(VteTerminal* terminal) {
    if (terminal != nullptr) {
        vte_terminal_copy_clipboard_format(terminal, VTE_FORMAT_TEXT);
    }
}

void paste(VteTerminal* terminal) {
    if (terminal != nullptr) {
        vte_terminal_paste_clipboard(terminal);
    }
}

}