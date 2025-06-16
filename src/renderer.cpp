#include "banim/renderer.h"
#include "banim/init.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cairo/cairo.h>

namespace banim {

// Upload the Cairo buffer into your texture
static void update_texture(cairo_surface_t *surface) {
    unsigned char *data = cairo_image_surface_get_data(surface);
    glBindTexture(GL_TEXTURE_2D, get_texture());
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width(), height(), GL_BGRA,
                    GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Draw a screen‐filling quad with your texture
static void draw_quad() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, get_texture());

    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex2f(-1, -1);
    glTexCoord2f(1, 1);
    glVertex2f(1, -1);
    glTexCoord2f(1, 0);
    glVertex2f(1, 1);
    glTexCoord2f(0, 0);
    glVertex2f(-1, 1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void draw(const std::function<void(cairo_t *)> &fn) {
    // 1) clear the Cairo surface
    cairo_t *cr = context();
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    // 2) user’s drawing code
    fn(cr);

    // 3) flush & upload to the GL texture
    cairo_surface_flush(surface());
    update_texture(surface());

    // 4) clear GL and draw the textured quad
    glClear(GL_COLOR_BUFFER_BIT);
    draw_quad();

    // 5) swap & poll
    glfwSwapBuffers(window());
    poll_events();
}

} // namespace banim
