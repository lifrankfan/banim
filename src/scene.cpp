#include "banim/scene.h"
#include "banim/shapes.h"
#include "banim/animations.h"
#include <algorithm>

namespace banim {

void Scene::add(std::shared_ptr<Shape> shape) {
    shapes_.push_back(shape);
    if (auto anim = shape->spawnAnimation()) {
        play(anim);
    }
}

void Scene::play(std::shared_ptr<Animation> anim) {
    animations_.push_back(anim);
}

void Scene::renderScene(cairo_t *cr) {
    for (auto &shape : shapes_) {
        shape->draw(cr);
    }
}

void Scene::update(float dt) {
    animations_.erase(std::remove_if(animations_.begin(), animations_.end(),
                                     [dt](std::shared_ptr<Animation> &anim) {
                                         return !anim->update(dt);
                                     }),
                      animations_.end());
}

} // namespace banim
