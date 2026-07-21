#pragma once

#include <string>

#include <gdk/gdk.h>

namespace blink::terminal {

std::string encode_key_event(GdkEventKey* event);

}
