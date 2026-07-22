#pragma once

#include <gtk/gtk.h>
#include <vte/vte.h>

namespace blink::menu {

GtkWidget* create_terminal_context_menu(VteTerminal* terminal);

}