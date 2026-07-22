#pragma once

#include <vte/vte.h>

namespace blink::clipboard {

void copy_selection(VteTerminal* terminal);
void paste(VteTerminal* terminal);
void select_all(VteTerminal* terminal);

}