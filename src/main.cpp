#include "blink/app/app.hpp"

int main(int argc, char** argv) {
    blink::app::BlinkApplication app(argc, argv);
    return app.run();
}
