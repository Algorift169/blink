#pragma once

#include <memory>
#include <string>

#include <glib.h>
#include <gtk/gtk.h>
#include <vte/vte.h>

#include "blink/terminal/pty.hpp"

namespace blink::terminal {

class TerminalWidget {
public:
    TerminalWidget();
    ~TerminalWidget();

    GtkWidget* widget() const;
    void start_shell();

private:
    static gboolean on_key_press(GtkWidget* widget, GdkEvent* event, gpointer data);
    static gboolean on_button_press(GtkWidget* widget, GdkEvent* event, gpointer data);
    static void on_commit(VteTerminal* terminal, const gchar* text, guint size,
                          gpointer data);
    static void on_size_allocate(GtkWidget* widget, GtkAllocation* allocation, gpointer data);

    void initialize();
    void apply_theme();
    void send_to_child(const char* data, std::size_t size);
    void copy_selection();
    void paste_from_clipboard();
    void show_context_menu(GdkEventButton* event);
    void resize_to_widget();

    GtkWidget* container_{nullptr};
    VteTerminal* terminal_{nullptr};
    std::unique_ptr<PtySession> session_;
};

void apply_terminal_theme(VteTerminal* terminal);
void configure_scrollback(VteTerminal* terminal, int lines);

}  
