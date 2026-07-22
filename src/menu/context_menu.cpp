#include <algorithm>

#include "blink/clipboard/actions.hpp"
#include "blink/menu/context_menu.hpp"

namespace blink::menu {

namespace {

constexpr gdouble kMinimumFontScale = 0.5;
constexpr gdouble kMaximumFontScale = 2.0;
constexpr gdouble kFontScaleStep = 0.1;
constexpr gdouble kDefaultFontScale = 0.94;

void on_copy(GtkMenuItem*, gpointer data) {
    blink::clipboard::copy_selection(VTE_TERMINAL(data));
}

void on_paste(GtkMenuItem*, gpointer data) {
    blink::clipboard::paste(VTE_TERMINAL(data));
}

void on_select_all(GtkMenuItem*, gpointer data) {
    blink::clipboard::select_all(VTE_TERMINAL(data));
}

void on_zoom_in(GtkMenuItem*, gpointer data) {
    auto* terminal = VTE_TERMINAL(data);
    const auto scale = std::min(kMaximumFontScale,
        vte_terminal_get_font_scale(terminal) + kFontScaleStep);
    vte_terminal_set_font_scale(terminal, scale);
}

void on_zoom_out(GtkMenuItem*, gpointer data) {
    auto* terminal = VTE_TERMINAL(data);
    const auto scale = std::max(kMinimumFontScale,
        vte_terminal_get_font_scale(terminal) - kFontScaleStep);
    vte_terminal_set_font_scale(terminal, scale);
}

void on_zoom_reset(GtkMenuItem*, gpointer data) {
    vte_terminal_set_font_scale(VTE_TERMINAL(data), kDefaultFontScale);
}

void add_item(GtkWidget* menu, const char* label,
              GCallback callback, VteTerminal* terminal) {
    auto* item = gtk_menu_item_new_with_label(label);
    g_signal_connect(item, "activate", callback, terminal);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
}

}

GtkWidget* create_terminal_context_menu(VteTerminal* terminal) {
    auto* menu = gtk_menu_new();
    add_item(menu, "Copy", G_CALLBACK(on_copy), terminal);
    add_item(menu, "Paste", G_CALLBACK(on_paste), terminal);
    add_item(menu, "Select All", G_CALLBACK(on_select_all), terminal);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());
    add_item(menu, "Zoom In", G_CALLBACK(on_zoom_in), terminal);
    add_item(menu, "Zoom Out", G_CALLBACK(on_zoom_out), terminal);
    add_item(menu, "Zoom Reset", G_CALLBACK(on_zoom_reset), terminal);
    gtk_widget_show_all(menu);
    return menu;
}

}