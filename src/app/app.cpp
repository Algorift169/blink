#include <memory>

#include <gtk/gtk.h>

#include "blink/app/app.hpp"

namespace blink::app {

BlinkApplication::BlinkApplication(int argc, char** argv)
    : argc_(argc), argv_(argv), app_(gtk_application_new("dev.blink.app", G_APPLICATION_FLAGS_NONE)) {
    g_signal_connect(app_, "activate", G_CALLBACK(on_activate), this);
}

BlinkApplication::~BlinkApplication() {
    if (app_ != nullptr) {
        g_object_unref(app_);
        app_ = nullptr;
    }
}

int BlinkApplication::run() {
    return g_application_run(G_APPLICATION(app_), argc_, argv_);
}

void BlinkApplication::on_activate(GtkApplication* app, gpointer user_data) {
    auto* self = static_cast<BlinkApplication*>(user_data);
    self->window_ = std::make_unique<blink::ui::BlinkWindow>(app);
    gtk_window_present(self->window_->window());
}

}  
