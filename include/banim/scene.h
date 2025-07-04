#pragma once

#include <cairo/cairo.h>
#include <memory>
#include <vector>
#include <queue>

namespace banim {

class Shape;
class Animation;

class Scene {
  public:
    void add(std::shared_ptr<Shape> shape);
    void play(std::shared_ptr<Animation> anim);
    void renderScene(cairo_t *cr);
    void update(float dt);
    void wait(float duration);

  private:
    std::vector<std::shared_ptr<Shape>> shapes_;
    std::queue<std::shared_ptr<Animation>> animationQueue_;
    std::shared_ptr<Animation> currentAnimation_ = nullptr;

};


} // namespace banim