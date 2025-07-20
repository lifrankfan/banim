#include "banim/scene.h"
#include "banim/shapes.h"
#include "banim/animations.h"
#include <algorithm>

namespace banim {

    void Scene::play(std::shared_ptr<Animation> anim) {
        timeline_.push(anim);
    }
    
    void Scene::wait(float duration) {
        timeline_.push(std::make_shared<Wait>(duration));
    }    

    void Scene::add(std::shared_ptr<Shape> shape) {
        // Create default PopIn animation and queue for timeline
        auto popIn = std::make_shared<PopIn>(shape, 0.5f);
        AddAction action{shape, popIn};
        timeline_.push(action);
    }

    void Scene::add(std::shared_ptr<Shape> shape, std::shared_ptr<Animation> animation) {
        AddAction action{shape, animation};
        timeline_.push(action);
    }

    void Scene::renderScene(cairo_t *cr) {
        for (auto &shape : shapes_) {
            shape->draw(cr);
        }
    }

    void Scene::update(float dt) {
        // Process timeline if no current animation
        if (!currentAnimation_ && !timeline_.empty()) {
            auto action = timeline_.front();
            timeline_.pop();
            
            // Check if this is an animation or a shape addition
            if (std::holds_alternative<std::shared_ptr<Animation>>(action)) {
                // It's an animation
                currentAnimation_ = std::get<std::shared_ptr<Animation>>(action);
            } else {
                // It's a shape addition
                auto addAction = std::get<AddAction>(action);
                
                // Add shape to the scene
                shapes_.push_back(addAction.shape);
                
                // If there's a spawn animation, start it
                if (addAction.spawnAnimation) {
                    addAction.shape->hide(); // Hide initially
                    currentAnimation_ = addAction.spawnAnimation;
                }
                // If no spawn animation, shape is immediately visible
            }
        }

        // Update current animation if any
        if (currentAnimation_) {
            if (!currentAnimation_->update(dt)) {
                currentAnimation_ = nullptr;
            }
        }
    }

} // namespace banim
