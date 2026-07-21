#include <algorithm>
#include <memory>
#include <string>

#include <gtk/gtk.h>
#include <vte/vte.h>

#include "blink/terminal/terminal.hpp"

namespace blink::terminal {

namespace {

constexpr int kScrollbackLines = 10000;

}  

TerminalWidget::TerminalWidget() {
    initialize();
}

TerminalWidget::~TerminalWidget() {
    if (session_) {
        session_->close();
    }
}

GtkWidget* TerminalWidget::widget() const {
    return container_;
}

void TerminalWidget::start_shell() {
    if (session_) {
        return;
    }

    session_ = std::make_unique<PtySession>([this](const std::string& data) {
        if (terminal_ != nullptr) {
            vte_terminal_feed(terminal_, data.data(), data.size());
        }
    });

    if (!session_->start()) {
        g_warning("Failed to start shell process");
        return;
    }

    resize_to_widget();
}

void TerminalWidget::initialize() {
    container_ = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    terminal_ = VTE_TERMINAL(vte_terminal_new());
    gtk_widget_set_name(GTK_WIDGET(terminal_), "blink-terminal");
    gtk_widget_set_hexpand(GTK_WIDGET(terminal_), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(terminal_), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(terminal_), TRUE);

    apply_terminal_theme(terminal_);
    configure_scrollback(terminal_, kScrollbackLines);
    vte_terminal_set_mouse_autohide(terminal_, TRUE);
    vte_terminal_set_allow_hyperlink(terminal_, FALSE);

    g_signal_connect(terminal_, "size-allocate", G_CALLBACK(on_size_allocate), this);
    g_signal_connect(terminal_, "key-press-event", G_CALLBACK(on_key_press), this);

    gtk_container_add(GTK_CONTAINER(container_), GTK_WIDGET(terminal_));
}

void TerminalWidget::apply_theme() {
    apply_terminal_theme(terminal_);
}

void TerminalWidget::send_to_child(const char* data, std::size_t size) {
    if (session_) {
        session_->write(data, size);
    }
}

void TerminalWidget::copy_selection() {
    if (terminal_ != nullptr) {
        vte_terminal_copy_clipboard(terminal_);
    }
}

void TerminalWidget::paste_from_clipboard() {
    if (terminal_ != nullptr) {
        vte_terminal_paste_clipboard(terminal_);
    }
}

void TerminalWidget::resize_to_widget() {
    if (terminal_ == nullptr || session_ == nullptr) {
        return;
    }

    int width = gtk_widget_get_allocated_width(GTK_WIDGET(terminal_));
    int height = gtk_widget_get_allocated_height(GTK_WIDGET(terminal_));
    int char_width = vte_terminal_get_char_width(terminal_);
    int char_height = vte_terminal_get_char_height(terminal_);
    if (char_width <= 0 || char_height <= 0) {
        return;
    }

    int cols = std::max(1, width / char_width);
    int rows = std::max(1, height / char_height);
    vte_terminal_set_size(terminal_, cols, rows);
    session_->resize(cols, rows);
}

gboolean TerminalWidget::on_key_press(GtkWidget* widget, GdkEvent* event, gpointer data) {
    auto* self = static_cast<TerminalWidget*>(data);
    guint keyval = 0;
    GdkModifierType state = static_cast<GdkModifierType>(0);
    gdk_event_get_keyval(event, &keyval);
    gdk_event_get_state(event, &state);

    if ((state & GDK_CONTROL_MASK) != 0 && (state & GDK_SHIFT_MASK) != 0 && keyval == GDK_KEY_c) {
        self->copy_selection();
        return TRUE;
    }

    if ((state & GDK_CONTROL_MASK) != 0 && (state & GDK_SHIFT_MASK) != 0 && keyval == GDK_KEY_v) {
        self->paste_from_clipboard();
        return TRUE;
    }

    if (keyval == GDK_KEY_Return || keyval == GDK_KEY_KP_Enter) {
        self->send_to_child("\r", 1);
        return TRUE;
    }

    if (keyval == GDK_KEY_Tab) {
        self->send_to_child("\t", 1);
        return TRUE;
    }

    if (keyval == GDK_KEY_BackSpace) {
        self->send_to_child("\x7f", 1);
        return TRUE;
    }

    if (keyval == GDK_KEY_Escape) {
        self->send_to_child("\x1b", 1);
        return TRUE;
    }

    if (keyval == GDK_KEY_Up) {
        self->send_to_child("\x1b[A", 3);
        return TRUE;
    }

    if (keyval == GDK_KEY_Down) {
        self->send_to_child("\x1b[B", 3);
        return TRUE;
    }

    if (keyval == GDK_KEY_Right) {
        self->send_to_child("\x1b[C", 3);
        return TRUE;
    }

    if (keyval == GDK_KEY_Left) {
        self->send_to_child("\x1b[D", 3);
        return TRUE;
    }

    if (keyval == GDK_KEY_Delete) {
        self->send_to_child("\x1b[3~", 4);
        return TRUE;
    }

    if (keyval >= 32 && keyval < 127) {
        gchar utf8[8]{};
        const auto length = g_unichar_to_utf8(gdk_keyval_to_unicode(keyval), utf8);
        self->send_to_child(utf8, static_cast<std::size_t>(length));
        return TRUE;
    }

    (void)widget;
    return FALSE;
}

void TerminalWidget::on_size_allocate(GtkWidget* widget, GtkAllocation* allocation, gpointer data) {
    auto* self = static_cast<TerminalWidget*>(data);
    (void)widget;
    (void)allocation;
    self->resize_to_widget();
}

void apply_terminal_theme(VteTerminal* terminal) {
    
    GdkRGBA foreground{1.0, 1.0, 1.0, 1.0};      
    GdkRGBA background{0.0, 0.0, 0.0, 1.0};     
    vte_terminal_set_colors(terminal, &foreground, &background, nullptr, 0);

    
    auto* font = pango_font_description_from_string("JetBrains Mono Nerd Font 9");
    if (font == nullptr) {
        font = pango_font_description_from_string("JetBrains Mono 9");
    }
    if (font == nullptr) {
        font = pango_font_description_from_string("Fira Code 9");
    }
    if (font == nullptr) {
        font = pango_font_description_from_string("Monospace 9");
    }
    vte_terminal_set_font(terminal, font);
    pango_font_description_free(font);

    vte_terminal_set_font_scale(terminal, 0.94);
    vte_terminal_set_cell_height_scale(terminal, 0.94);
    vte_terminal_set_cursor_shape(terminal, VTE_CURSOR_SHAPE_IBEAM);
    vte_terminal_set_cursor_blink_mode(terminal, VTE_CURSOR_BLINK_OFF);
}

void configure_scrollback(VteTerminal* terminal, int lines) {
    vte_terminal_set_scrollback_lines(terminal, lines);
}

}  
