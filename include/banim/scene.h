#pragma once

#include <cairo/cairo.h>
#include <memory>
#include <vector>

namespace banim {

class Shape;
class Animation;

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