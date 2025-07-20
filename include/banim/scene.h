#pragma once

#include <cairo/cairo.h>
#include <memory>
#include <vector>
#include <queue>
#include <variant>

namespace banim {

class Shape;
class Animation;

struct AddAction {
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Animation> spawnAnimation; // nullptr for no animation
};

using TimelineAction = std::variant<std::shared_ptr<Animation>, AddAction>;

class Scene {
  public:
    void add(std::shared_ptr<Shape> shape);
    
    void add(std::shared_ptr<Shape> shape, std::shared_ptr<Animation> animation);
    
    void play(std::shared_ptr<Animation> anim);
    void renderScene(cairo_t *cr);
    void update(float dt);
    void wait(float duration);

  private:
    std::vector<std::shared_ptr<Shape>> shapes_;
    std::queue<TimelineAction> timeline_;
    std::shared_ptr<Animation> currentAnimation_ = nullptr;

};


} // namespace banim