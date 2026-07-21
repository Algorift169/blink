#pragma once

#include <memory>

#include <gtk/gtk.h>

#include "blink/terminal/terminal.hpp"

namespace blink::ui {

class BlinkWindow {
public:
    explicit BlinkWindow(GtkApplication* app);
    ~BlinkWindow();

    GtkWindow* window() const;

private:
    void apply_css();
    void create_title_bar();
    void setup_window_decorations();

    GtkApplication* app_{nullptr};
    GtkWindow* window_{nullptr};
    GtkBox* title_bar_{nullptr};
    std::unique_ptr<blink::terminal::TerminalWidget> terminal_widget_;
};

}  
