#include <string>

#include <gdk/gdkkeysyms.h>

#include "blink/terminal/input.hpp"

namespace blink::terminal {

namespace {

std::string control_sequence(guint keyval) {
    if (keyval >= GDK_KEY_a && keyval <= GDK_KEY_z) {
        return std::string(1, static_cast<char>(keyval - GDK_KEY_a + 1));
    }
    if (keyval >= GDK_KEY_A && keyval <= GDK_KEY_Z) {
        return std::string(1, static_cast<char>(keyval - GDK_KEY_A + 1));
    }

    switch (keyval) {
    case GDK_KEY_space:
    case GDK_KEY_2:
    case GDK_KEY_at:
        return std::string(1, '\0');
    case GDK_KEY_3:
    case GDK_KEY_bracketleft:
        return "\x1b";
    case GDK_KEY_4:
    case GDK_KEY_backslash:
        return "\x1c";
    case GDK_KEY_5:
    case GDK_KEY_bracketright:
        return "\x1d";
    case GDK_KEY_6:
    case GDK_KEY_asciicircum:
        return "\x1e";
    case GDK_KEY_7:
    case GDK_KEY_underscore:
    case GDK_KEY_question:
        return "\x1f";
    case GDK_KEY_8:
        return "\0";
    case GDK_KEY_BackSpace:
        return "\x08";
    case GDK_KEY_Delete:
        return "\x7f";
    default:
        return {};
    }
}

std::string function_key_sequence(guint keyval) {
    switch (keyval) {
    case GDK_KEY_F1: return "\x1bOP";
    case GDK_KEY_F2: return "\x1bOQ";
    case GDK_KEY_F3: return "\x1bOR";
    case GDK_KEY_F4: return "\x1bOS";
    case GDK_KEY_F5: return "\x1b[15~";
    case GDK_KEY_F6: return "\x1b[17~";
    case GDK_KEY_F7: return "\x1b[18~";
    case GDK_KEY_F8: return "\x1b[19~";
    case GDK_KEY_F9: return "\x1b[20~";
    case GDK_KEY_F10: return "\x1b[21~";
    case GDK_KEY_F11: return "\x1b[23~";
    case GDK_KEY_F12: return "\x1b[24~";
    default: return {};
    }
}

std::string navigation_sequence(guint keyval) {
    switch (keyval) {
    case GDK_KEY_Up: return "\x1b[A";
    case GDK_KEY_Down: return "\x1b[B";
    case GDK_KEY_Right: return "\x1b[C";
    case GDK_KEY_Left: return "\x1b[D";
    case GDK_KEY_Home: return "\x1b[H";
    case GDK_KEY_End: return "\x1b[F";
    case GDK_KEY_Page_Up: return "\x1b[5~";
    case GDK_KEY_Page_Down: return "\x1b[6~";
    case GDK_KEY_Insert: return "\x1b[2~";
    case GDK_KEY_Delete: return "\x1b[3~";
    default: return {};
    }
}

}  // namespace

std::string encode_key_event(GdkEventKey* event) {
    if (event == nullptr) {
        return {};
    }

    const auto state = static_cast<GdkModifierType>(event->state &
        (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK | GDK_SUPER_MASK | GDK_META_MASK));
    const bool control = (state & GDK_CONTROL_MASK) != 0;
    const bool alt = (state & GDK_MOD1_MASK) != 0;
    const bool shift = (state & GDK_SHIFT_MASK) != 0;
    const guint keyval = event->keyval;

    if (control) {
        auto sequence = control_sequence(keyval);
        if (!sequence.empty()) {
            return sequence;
        }
    }

    if (keyval == GDK_KEY_Return || keyval == GDK_KEY_KP_Enter) return "\r";
    if (keyval == GDK_KEY_Tab) return "\t";
    if (keyval == GDK_KEY_BackSpace) return "\x7f";
    if (keyval == GDK_KEY_Escape) return "\x1b";

    auto sequence = navigation_sequence(keyval);
    if (sequence.empty()) {
        sequence = function_key_sequence(keyval);
    }
    if (!sequence.empty()) {
        if (alt) return "\x1b" + sequence;
        return sequence;
    }

    const gunichar unicode = gdk_keyval_to_unicode(keyval);
    if (unicode == 0 || unicode < 32 || unicode == 127) {
        return {};
    }

    gchar utf8[8]{};
    const auto length = g_unichar_to_utf8(unicode, utf8);
    std::string text(utf8, static_cast<std::size_t>(length));
    if (alt) {
        text.insert(text.begin(), '\x1b');
    }
    (void)shift;
    return text;
}

}
