#include "banim/scene.h"
#include "banim/shapes.h"
#include <algorithm>

namespace banim {

MoveTo::MoveTo(std::shared_ptr<Shape> target, float toX, float toY,
               float duration)
    : target_(target), toX_(toX), toY_(toY), duration_(duration) {}

bool MoveTo::update(float dt) {
    if (!initialized_) {
        fromX_ = target_->x();
        fromY_ = target_->y();
        initialized_ = true;
    }
    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f)
        t = 1.0f;

    float newX = fromX_ + (toX_ - fromX_) * t;
    float newY = fromY_ + (toY_ - fromY_) * t;
    target_->setPos(newX, newY);

    return t < 1.0f;
}

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
