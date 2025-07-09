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
        if (auto anim = shape->spawnAnimation()) { // checks if shape has a spawn animation
            shape->hide(); // hide the shape initially
            shape->setSpawnAnimation(nullptr); // clear the spawn animation after adding it to the queue
            animationQueue_.push(anim);

        }
    }


    void Scene::renderScene(cairo_t *cr) {
        for (auto &shape : shapes_) {
            shape->draw(cr);
        }
    }

    void Scene::update(float dt) {
    // Run current animation from the timeline
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
