#include "banim/scene.h"
#include "banim/animatable.h"
#include "banim/animations.h"
#include "banim/init.h"
#include <algorithm>
#include <cmath>

namespace banim {

    std::pair<float, float> Scene::gridToPixel(const GridCoord& coord) const {
        return gridToPixel(coord.x, coord.y);
    }
    
    std::pair<float, float> Scene::gridToPixel(float gridX, float gridY) const {
        if (!g_ctx) return {0, 0};
        
        float windowWidth = static_cast<float>(g_ctx->width());
        float windowHeight = static_cast<float>(g_ctx->height());
        
        float cellWidth = windowWidth / gridConfig_.cols;
        float cellHeight = windowHeight / gridConfig_.rows;
        
        // Grid coordinates are centered in cells
        float pixelX = (gridX + 0.5f) * cellWidth;
        float pixelY = (gridY + 0.5f) * cellHeight;
        
        return {pixelX, pixelY};
    }
    
    std::pair<float, float> Scene::getGridCellSize() const {
        if (!g_ctx) return {50, 50}; // Default fallback
        
        float windowWidth = static_cast<float>(g_ctx->width());
        float windowHeight = static_cast<float>(g_ctx->height());
        
        return {windowWidth / gridConfig_.cols, windowHeight / gridConfig_.rows};
    }
    
    void Scene::drawGrid(cairo_t *cr) const {
        if (!gridConfig_.displayGrid || !g_ctx) return;
        
        float windowWidth = static_cast<float>(g_ctx->width());
        float windowHeight = static_cast<float>(g_ctx->height());
        
        float cellWidth = windowWidth / gridConfig_.cols;
        float cellHeight = windowHeight / gridConfig_.rows;
        
        cairo_save(cr);
        cairo_set_source_rgba(cr, gridConfig_.r, gridConfig_.g, gridConfig_.b, gridConfig_.a);
        cairo_set_line_width(cr, gridConfig_.lineWidth);
        
        // Draw vertical lines
        for (int i = 0; i <= gridConfig_.cols; ++i) {
            float x = i * cellWidth;
            cairo_move_to(cr, x, 0);
            cairo_line_to(cr, x, windowHeight);
            cairo_stroke(cr);
        }
        
        // Draw horizontal lines
        for (int i = 0; i <= gridConfig_.rows; ++i) {
            float y = i * cellHeight;
            cairo_move_to(cr, 0, y);
            cairo_line_to(cr, windowWidth, y);
            cairo_stroke(cr);
        }
        
        cairo_restore(cr);
    }

    void Scene::play(std::shared_ptr<Animation> anim) {
        timeline_.push(anim);
    }
    
    void Scene::playGroup(const std::vector<std::shared_ptr<Animation>>& animations) {
        if (animations.empty()) return;
        
        auto group = std::make_shared<AnimationGroup>();
        group->addAll(animations);
        timeline_.push(group);
    }
    
    void Scene::wait(float duration) {
        timeline_.push(std::make_shared<Wait>(duration));
    }    

    void Scene::add(std::shared_ptr<Animatable> animatable) {
        // Create default PopIn animation and queue for timeline
        auto popIn = std::make_shared<PopIn>(animatable, 0.5f);
        AddAction action{animatable, popIn};
        timeline_.push(action);
    }

    void Scene::add(std::shared_ptr<Animatable> animatable, std::shared_ptr<Animation> animation) {
        AddAction action{animatable, animation};
        timeline_.push(action);
    }

    void Scene::renderScene(cairo_t *cr) {
        // Draw grid first (behind everything)
        drawGrid(cr);
        
        // Draw all animatable objects
        for (auto &animatable : animatables_) {
            animatable->draw(cr);
        }
    }

    void Scene::update(float dt) {
        // Process timeline if no current animation
        if (!currentAnimation_ && !timeline_.empty()) {
            auto action = timeline_.front();
            timeline_.pop();
            
            // Check if this is an animation or an animatable addition
            if (std::holds_alternative<std::shared_ptr<Animation>>(action)) {
                // It's an animation
                currentAnimation_ = std::get<std::shared_ptr<Animation>>(action);
            } else {
                // It's an animatable addition
                auto addAction = std::get<AddAction>(action);
                
                // Add animatable to the scene
                animatables_.push_back(addAction.animatable);
                
                // If there's a spawn animation, start it
                if (addAction.spawnAnimation) {
                    addAction.animatable->hide(); // Hide initially
                    currentAnimation_ = addAction.spawnAnimation;
                }
                // If no spawn animation, animatable is immediately visible
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
