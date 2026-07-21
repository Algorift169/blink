#include <gtk/gtk.h>
#include <iostream>

#include "blink/ui/window.hpp"

namespace blink::ui {

namespace {

gboolean on_window_button_press(GtkWidget* widget __attribute__((unused)), GdkEvent* event, gpointer user_data) {
    auto* window = static_cast<GtkWindow*>(user_data);
    GdkEventButton* button_event = reinterpret_cast<GdkEventButton*>(event);
    
    
    if (button_event->type == GDK_BUTTON_PRESS && button_event->button == 1 && button_event->y < 28) {
        if (button_event->x > 50) {  
            gtk_window_begin_move_drag(window, 1, button_event->x_root, button_event->y_root, button_event->time);
            return TRUE;
        }
    }
    return FALSE;
}

void on_close_clicked(GtkButton* button __attribute__((unused)), gpointer user_data) {
    auto* window = static_cast<GtkWindow*>(user_data);
    gtk_window_close(window);
}

void on_minimize_clicked(GtkButton* button __attribute__((unused)), gpointer user_data) {
    auto* window = static_cast<GtkWindow*>(user_data);
    gtk_window_iconify(window);
}

void on_maximize_clicked(GtkButton* button __attribute__((unused)), gpointer user_data) {
    auto* window = static_cast<GtkWindow*>(user_data);
    if (gdk_window_get_state(gtk_widget_get_window(GTK_WIDGET(window))) & GDK_WINDOW_STATE_MAXIMIZED) {
        gtk_window_unmaximize(window);
    } else {
        gtk_window_maximize(window);
    }
}

}  

BlinkWindow::BlinkWindow(GtkApplication* app) : app_(app) {
    window_ = GTK_WINDOW(gtk_application_window_new(app));
    gtk_window_set_title(window_, "Blink");
    gtk_window_set_default_size(window_, 800, 500);
    gtk_window_set_resizable(window_, TRUE);
    gtk_window_set_decorated(window_, FALSE);
    gtk_window_set_keep_above(window_, FALSE);
    gtk_widget_add_events(GTK_WIDGET(window_), GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_BUTTON_MOTION_MASK);
    g_signal_connect(GTK_WIDGET(window_), "button-press-event", G_CALLBACK(on_window_button_press), window_);

    terminal_widget_ = std::make_unique<blink::terminal::TerminalWidget>();

    setup_window_decorations();
    apply_css();

    terminal_widget_->start_shell();
}

BlinkWindow::~BlinkWindow() = default;

GtkWindow* BlinkWindow::window() const {
    return window_;
}

void BlinkWindow::create_title_bar() {
    auto* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(main_box, "blink-container");

    title_bar_ = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_widget_set_name(GTK_WIDGET(title_bar_), "blink-titlebar");
    gtk_widget_set_size_request(GTK_WIDGET(title_bar_), -1, 28);
    gtk_box_set_homogeneous(title_bar_, FALSE);
    
    auto* left_spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(left_spacer, 50, -1);
    gtk_box_pack_start(title_bar_, left_spacer, FALSE, FALSE, 0);
    
    auto* title_label = gtk_label_new("Blink");
    gtk_widget_set_name(title_label, "blink-title");
    gtk_label_set_xalign(GTK_LABEL(title_label), 0.5);
    gtk_widget_set_hexpand(title_label, TRUE);
    gtk_box_pack_start(title_bar_, title_label, TRUE, TRUE, 0);
    
    auto* right_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_margin_end(right_box, 12);
    gtk_widget_set_valign(right_box, GTK_ALIGN_CENTER);
    
    auto* minimize_btn = gtk_button_new();
    gtk_widget_set_name(minimize_btn, "minimize-button");
    gtk_widget_set_size_request(minimize_btn, 5, 5);
    gtk_button_set_relief(GTK_BUTTON(minimize_btn), GTK_RELIEF_NONE);
    g_signal_connect(minimize_btn, "clicked", G_CALLBACK(on_minimize_clicked), window_);
    gtk_box_pack_start(GTK_BOX(right_box), minimize_btn, FALSE, FALSE, 0);

    auto* maximize_btn = gtk_button_new();
    gtk_widget_set_name(maximize_btn, "maximize-button");
    gtk_widget_set_size_request(maximize_btn, 5, 5);
    gtk_button_set_relief(GTK_BUTTON(maximize_btn), GTK_RELIEF_NONE);
    g_signal_connect(maximize_btn, "clicked", G_CALLBACK(on_maximize_clicked), window_);
    gtk_box_pack_start(GTK_BOX(right_box), maximize_btn, FALSE, FALSE, 0);

    auto* close_btn = gtk_button_new();
    gtk_widget_set_name(close_btn, "close-button");
    gtk_widget_set_size_request(close_btn, 5, 5);
    gtk_button_set_relief(GTK_BUTTON(close_btn), GTK_RELIEF_NONE);
    g_signal_connect(close_btn, "clicked", G_CALLBACK(on_close_clicked), window_);
    gtk_box_pack_start(GTK_BOX(right_box), close_btn, FALSE, FALSE, 0);
    
    gtk_box_pack_end(title_bar_, right_box, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(main_box), GTK_WIDGET(title_bar_), FALSE, FALSE, 0);
    
    auto* terminal_container = terminal_widget_->widget();
    gtk_widget_set_name(terminal_container, "blink-terminal");
    gtk_widget_set_hexpand(terminal_container, TRUE);
    gtk_widget_set_vexpand(terminal_container, TRUE);
    gtk_box_pack_start(GTK_BOX(main_box), terminal_container, TRUE, TRUE, 0);
    
    gtk_container_add(GTK_CONTAINER(window_), main_box);
    gtk_widget_show_all(GTK_WIDGET(window_));
}

void BlinkWindow::setup_window_decorations() {
    gtk_window_set_decorated(window_, FALSE);
}

void BlinkWindow::apply_css() {
    create_title_bar();
    
    auto* provider = gtk_css_provider_new();
    if (!gtk_css_provider_load_from_path(provider, "assets/css/blink.css", nullptr)) {
        g_warning("Failed to load blink.css, falling back to inline CSS");
        const char* fallback_css =
            "window { background-color: #000000; border: 1px solid #333333; border-radius: 18px; }"
            "#blink-container { background-color: #000000; border: 1px solid #333333; border-radius: 18px; overflow: hidden; }"
            "#blink-titlebar { background-color: #1C1C1E; min-height: 28px; border-top-left-radius: 18px; border-top-right-radius: 18px; }"
            "#blink-terminal { background-color: #000000; color: #FFFFFF; padding: 12px; border-bottom-left-radius: 18px; border-bottom-right-radius: 18px; }"
            ".traffic-light-button { min-width: 5px; min-height: 5px; border-radius: 2.5px; border: none; margin: 0 4px; }"
            "#close-button { background-color: #FF5F57; min-width: 5px; min-height: 5px; border-radius: 2.5px; }"
            "#minimize-button { background-color: #FEBC2E; min-width: 5px; min-height: 5px; border-radius: 2.5px; }"
            "#maximize-button { background-color: #28C840; min-width: 5px; min-height: 5px; border-radius: 2.5px; }"
            "#blink-title { color: #D9D9D9; font-size: 12px; font-weight: 500; }";
        gtk_css_provider_load_from_data(provider, fallback_css, -1, nullptr);
    }
    
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

}  
