#include "banim/init.h"
#include <cstring>
#include <iostream>
#include <vector>

#define GL_CHECK()                                                             \
    do {                                                                       \
        GLenum err = glGetError();                                             \
        if (err != GL_NO_ERROR)                                                \
            std::cerr << "GL Error: " << err << std::endl;                     \
    } while (0)

namespace banim {

GLContext *g_ctx = nullptr;
CairoSurface *g_cairo = nullptr;
Texture2D *g_tex = nullptr;
Shader *g_shader = nullptr;
GLuint g_vao = 0, g_vbo = 0;

static const char *kVertShader = R"glsl(
#version 120
attribute vec2 aPos; attribute vec2 aUV; varying vec2 vUV;
void main() { vUV = aUV; gl_Position = vec4(aPos, 0.0, 1.0); }
)glsl";

static const char *kFragShader = R"glsl(
#version 120
uniform sampler2D uTex; varying vec2 vUV;
void main() { gl_FragColor = texture2D(uTex, vUV); }
)glsl";

GLContext::GLContext(int w, int h, const char *title, bool vsync)
    : w_(w), h_(h) {
    if (!glfwInit())
        throw std::runtime_error("Failed to init GLFW");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    win_ = glfwCreateWindow(w_, h_, title, nullptr, nullptr);
    if (!win_) {
        glfwTerminate();
        throw std::runtime_error("Window creation failed");
    }
    glfwMakeContextCurrent(win_);
    glfwSwapInterval(vsync ? 1 : 0);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw std::runtime_error("Failed to init GLEW");
    glViewport(0, 0, w_, h_);
    glfwSetFramebufferSizeCallback(win_, framebufferResizeCallback);
}

GLContext::~GLContext() {
    if (win_)
        glfwDestroyWindow(win_);
    glfwTerminate();
}

void GLContext::framebufferResizeCallback(GLFWwindow *, int fw, int fh) {
    if (!g_ctx)
        return;
    g_ctx->w_ = fw;
    g_ctx->h_ = fh;
    glViewport(0, 0, fw, fh);
    g_cairo->recreate(fw, fh);
    g_tex->resize(fw, fh);
}

CairoSurface::CairoSurface(int w, int h) { recreate(w, h); }
CairoSurface::~CairoSurface() {
    if (cr_)
        cairo_destroy(cr_);
    if (surf_)
        cairo_surface_destroy(surf_);
}
void CairoSurface::recreate(int w, int h) {
    if (cr_) {
        cairo_destroy(cr_);
        cr_ = nullptr;
    }
    if (surf_) {
        cairo_surface_destroy(surf_);
        surf_ = nullptr;
    }
    surf_ = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
    if (cairo_surface_status(surf_) != CAIRO_STATUS_SUCCESS)
        throw std::runtime_error("Cairo surface creation failed");
    cr_ = cairo_create(surf_);
    if (cairo_status(cr_) != CAIRO_STATUS_SUCCESS)
        throw std::runtime_error("Cairo context creation failed");
}

Texture2D::Texture2D(int w, int h) : tex_w_(w), tex_h_(h) {
    glGenTextures(1, &tex_);
    glBindTexture(GL_TEXTURE_2D, tex_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_w_, tex_h_, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, nullptr);
    glGenBuffers(1, &pbo_);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, tex_w_ * tex_h_ * 4, nullptr,
                 GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}
Texture2D::~Texture2D() {
    if (pbo_)
        glDeleteBuffers(1, &pbo_);
    if (tex_)
        glDeleteTextures(1, &tex_);
}
void Texture2D::resize(int w, int h) {
    tex_w_ = w;
    tex_h_ = h;
    glBindTexture(GL_TEXTURE_2D, tex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE,
                 nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, w * h * 4, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}
void Texture2D::upload(const cairo_surface_t *surf) {
    int stride = cairo_image_surface_get_stride((cairo_surface_t *)surf);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, stride / 4);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, tex_w_ * tex_h_ * 4, nullptr,
                 GL_STREAM_DRAW);
    void *ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, cairo_image_surface_get_data((cairo_surface_t *)surf),
           tex_w_ * tex_h_ * 4);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    glBindTexture(GL_TEXTURE_2D, tex_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex_w_, tex_h_, GL_BGRA,
                    GL_UNSIGNED_BYTE, nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

Shader::Shader(const char *vsrc, const char *fsrc) {
    GLuint vs = compile(GL_VERTEX_SHADER, vsrc);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fsrc);
    prog_ = glCreateProgram();
    glAttachShader(prog_, vs);
    glAttachShader(prog_, fs);
    glLinkProgram(prog_);
    GLint ok;
    glGetProgramiv(prog_, GL_LINK_STATUS, &ok);
    if (!ok) {
        char buf[512];
        glGetProgramInfoLog(prog_, 512, nullptr, buf);
        throw std::runtime_error(std::string("Shader link failed: ") + buf);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
}
Shader::~Shader() {
    if (prog_)
        glDeleteProgram(prog_);
}
GLuint Shader::compile(GLenum t, const char *src) {
    GLuint s = glCreateShader(t);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char buf[512];
        glGetShaderInfoLog(s, 512, nullptr, buf);
        throw std::runtime_error(std::string("Shader compile failed: ") + buf);
    }
    return s;
}

bool init(const InitOptions &opt) {
    try {
        g_ctx = new GLContext(opt.width, opt.height, opt.title, opt.vsync);
        g_cairo = new CairoSurface(opt.width, opt.height);
        g_tex = new Texture2D(opt.width, opt.height);
        g_shader = new Shader(kVertShader, kFragShader);
        float quad[] = {-1, -1, 0, 1, 1, -1, 1, 1, 1, 1, 1, 0, -1, 1, 0, 0};
        glGenVertexArrays(1, &g_vao);
        glGenBuffers(1, &g_vbo);
        glBindVertexArray(g_vao);
        glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
        GLint posLoc = glGetAttribLocation(g_shader->id(), "aPos");
        GLint uvLoc = glGetAttribLocation(g_shader->id(), "aUV");
        glEnableVertexAttribArray(posLoc);
        glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(uvLoc);
        glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              (void *)(2 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    } catch (...) {
        return false;
    }
    return true;
}

void Scene::render() {
    cairo_t *cr = g_cairo->context();
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    for (auto &shape : shapes_)
        shape->draw(cr);
    cairo_surface_flush(g_cairo->surface());
    g_tex->upload(g_cairo->surface());
    glClear(GL_COLOR_BUFFER_BIT);
    g_shader->use();
    glBindVertexArray(g_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_tex->id());
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    glfwSwapBuffers(g_ctx->window());
    glfwPollEvents();
}

void renderScene(Scene &scene) { scene.render(); }

void cleanup() {
    delete g_shader;
    g_shader = nullptr;
    delete g_tex;
    g_tex = nullptr;
    delete g_cairo;
    g_cairo = nullptr;
    delete g_ctx;
    g_ctx = nullptr;
    if (g_vao)
        glDeleteVertexArrays(1, &g_vao);
    if (g_vbo)
        glDeleteBuffers(1, &g_vbo);
}

} // namespace banim
