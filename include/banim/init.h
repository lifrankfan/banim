// banim/init.h
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cairo/cairo.h>
#include <functional>
#include <stdexcept>
#include <string>

namespace banim {

struct InitOptions {
    int width;
    int height;
    const char *title;
    bool vsync;
};

// RAII wrapper for GLFW + GLEW context
class GLContext {
  public:
    GLContext(int w, int h, const char *title, bool vsync);
    ~GLContext();
    GLFWwindow *window() const { return win_; }
    int width() const { return w_; }
    int height() const { return h_; }
    static void framebufferResizeCallback(GLFWwindow *w, int fw, int fh);

  private:
    GLFWwindow *win_ = nullptr;
    int w_, h_;
};

// RAII wrapper for Cairo surface + context
class CairoSurface {
  public:
    CairoSurface(int w, int h);
    ~CairoSurface();
    void recreate(int w, int h);
    cairo_surface_t *surface() const { return surf_; }
    cairo_t *context() const { return cr_; }

  private:
    cairo_surface_t *surf_ = nullptr;
    cairo_t *cr_ = nullptr;
};

// RAII wrapper for texture + optional PBO
class Texture2D {
  public:
    Texture2D(int w, int h);
    ~Texture2D();
    void resize(int w, int h);
    void upload(const cairo_surface_t *surf);
    GLuint id() const { return tex_; }

  private:
    int tex_w_, tex_h_;
    GLuint tex_ = 0;
    GLuint pbo_ = 0;
};

// Simple shader program wrapper
class Shader {
  public:
    Shader(const char *vertSrc, const char *fragSrc);
    ~Shader();
    void use() const { glUseProgram(prog_); }
    GLuint id() const { return prog_; }

  private:
    GLuint compile(GLenum type, const char *src);
    GLuint prog_ = 0;
};

// Main init/draw/cleanup API
bool init(const InitOptions &opt);
void draw(const std::function<void(cairo_t *)> &fn);
void cleanup();

} // namespace banim
