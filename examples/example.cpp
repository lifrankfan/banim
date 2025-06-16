// main.cpp
#include "banim/init.h"
#include <GLFW/glfw3.h>
#include <cmath>

int main() {
    // 1) Set up the window, GL, Cairo, texture, shaders, etc.
    banim::InitOptions opts{
        800,          // width
        600,          // height
        "Banim Demo", // window title
        true          // vsync on
    };
    if (!banim::init(opts)) {
        return -1;
    }

    // 2) Render loop
    while (!glfwWindowShouldClose(glfwGetCurrentContext())) {
        banim::draw([](cairo_t *cr) {
            // clear → (already done by banim)

            // Draw a filled blue square
            cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
            cairo_rectangle(cr, 100.0, 100.0, 200.0, 200.0);
            cairo_fill(cr);

            // Draw a filled red circle
            const double cx = 500.0, cy = 350.0, radius = 100.0;
            cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
            cairo_arc(cr, cx, cy, radius, 0.0, 2.0 * M_PI);
            cairo_fill(cr);
        });
    }

    // 3) Tear everything down
    banim::cleanup();
    return 0;
}
