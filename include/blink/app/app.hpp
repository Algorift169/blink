#pragma once

#include <memory>

#include <gtk/gtk.h>

#include "blink/ui/window.hpp"

namespace blink::app {

class BlinkApplication {
public:
    BlinkApplication(int argc, char** argv);
    ~BlinkApplication();

    int run();

private:
    static void on_activate(GtkApplication* app, gpointer user_data);

    int argc_{0};
    char** argv_{nullptr};
    GtkApplication* app_{nullptr};
    std::unique_ptr<blink::ui::BlinkWindow> window_;
};

}  
