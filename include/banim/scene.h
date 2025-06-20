#pragma once

#include <cairo/cairo.h>
#include <memory>
#include <vector>

namespace banim {

class Shape;

class Animation {
  public:
    virtual ~Animation() = default;
    virtual bool update(float dt) = 0;
};

class MoveTo : public Animation {
  public:
    MoveTo(std::shared_ptr<Shape> target, float toX, float toY, float duration);
    bool update(float dt) override;

  private:
    std::shared_ptr<Shape> target_;
    float fromX_, fromY_;
    float toX_, toY_;
    float duration_;
    float elapsed_ = 0.0f;
    bool initialized_ = false;
};

class Scene {
  public:
    void add(std::shared_ptr<Shape> shape);
    void play(std::shared_ptr<Animation> anim);
    void renderScene(cairo_t *cr);
    void update(float dt);

  private:
    std::vector<std::shared_ptr<Shape>> shapes_;
    std::vector<std::shared_ptr<Animation>> animations_;
};

} // namespace banim
