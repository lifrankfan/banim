#pragma once

#include "banim/shapes.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cairo.h>
#include <functional>
#include <memory>
#include <vector>

namespace banim {

struct InitOptions {
    int width;
    int height;
    const char *title;
    bool vsync;
};

class GLContext {
  public:
    GLContext(int w, int h, const char *title, bool vsync);
    ~GLContext();
    GLFWwindow *window() const { return win_; }
    static void framebufferResizeCallback(GLFWwindow *w, int fw, int fh);
    int width() const { return w_; }
    int height() const { return h_; }

  private:
    GLFWwindow *win_ = nullptr;
    int w_, h_;
};

class CairoSurface {
  public:
    CairoSurface(int w, int h);
    ~CairoSurface();
    void recreate(int w, int h);
    cairo_t *context() const { return cr_; }
    cairo_surface_t *surface() const { return surf_; }

  private:
    cairo_surface_t *surf_ = nullptr;
    cairo_t *cr_ = nullptr;
};

class Texture2D {
  public:
    Texture2D(int w, int h);
    ~Texture2D();
    void resize(int w, int h);
    void upload(const cairo_surface_t *surf);
    GLuint id() const { return tex_; }

  private:
    GLuint tex_ = 0, pbo_ = 0;
    int tex_w_, tex_h_;
};

class Shader {
  public:
    Shader(const char *vsrc, const char *fsrc);
    ~Shader();
    GLuint id() const { return prog_; }
    void use() const { glUseProgram(prog_); }

  private:
    GLuint compile(GLenum t, const char *src);
    GLuint prog_ = 0;
};

class Scene {
  public:
    void add(std::shared_ptr<Shape> shape) { shapes_.push_back(shape); }
    void render();

  private:
    std::vector<std::shared_ptr<Shape>> shapes_;
};

bool init(const InitOptions &opt);
void renderScene(Scene &scene);
void cleanup();

extern GLContext *g_ctx;

} // namespace banim
