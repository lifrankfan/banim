#include "banim/scene.h"
#include "banim/shapes.h"
#include "banim/animations.h"
#include <algorithm>

namespace banim {

    void Scene::play(std::shared_ptr<Animation> anim) {
        animationQueue_.push(anim);
    }
    
    void Scene::wait(float duration) {
        animationQueue_.push(std::make_shared<Wait>(duration));
    }    

    void Scene::add(std::shared_ptr<Shape> shape) {
        shapes_.push_back(shape);
        
        shape->initDefaultSpawn(0.5f);
        
        if (auto anim = shape->spawnAnimation()) {
            shape->hide();
            shape->setSpawnAnimation(nullptr);
            animationQueue_.push(anim);
        }
    }


    void Scene::renderScene(cairo_t *cr) {
        for (auto &shape : shapes_) {
            shape->draw(cr);
        }
    }

    void Scene::update(float dt) {
    if (!currentAnimation_ && !animationQueue_.empty()) {
        currentAnimation_ = animationQueue_.front();
        animationQueue_.pop();
    }

    if (currentAnimation_) {
        if (!currentAnimation_->update(dt)) {
            currentAnimation_ = nullptr;
        }
    }
}


} // namespace banim
