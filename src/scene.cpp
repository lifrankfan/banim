#include "banim/scene.h"
#include "banim/shapes.h"
#include "banim/animations.h"
#include "banim/init.h"
#include <algorithm>
#include <cmath>

namespace banim {

    std::pair<float, float> Scene::gridToPixel(const GridCoord& coord) const {
        return gridToPixel(coord.x, coord.y);
    }
    
    std::pair<float, float> Scene::gridToPixel(float gridX, float gridY) const {
        // Use fixed reference size for consistent grid scaling
        float cellWidth = gridConfig_.referenceWidth / gridConfig_.cols;
        float cellHeight = gridConfig_.referenceHeight / gridConfig_.rows;
        
        // Grid coordinates are centered in cells
        float pixelX = (gridX + 0.5f) * cellWidth;
        float pixelY = (gridY + 0.5f) * cellHeight;
        
        return {pixelX, pixelY};
    }
    
    GridCoord Scene::pixelToGrid(float pixelX, float pixelY) const {
        // Use fixed reference size for consistent grid scaling
        float cellWidth = gridConfig_.referenceWidth / gridConfig_.cols;
        float cellHeight = gridConfig_.referenceHeight / gridConfig_.rows;
        
        float gridX = (pixelX / cellWidth) - 0.5f;
        float gridY = (pixelY / cellHeight) - 0.5f;
        
        return {gridX, gridY};
    }
    
    std::pair<float, float> Scene::getGridCellSize() const {
        // Use fixed reference size for consistent grid scaling
        return {gridConfig_.referenceWidth / gridConfig_.cols, 
                gridConfig_.referenceHeight / gridConfig_.rows};
    }
    
    void Scene::drawGrid(cairo_t *cr) const {
        if (!gridConfig_.displayGrid || !g_ctx) return;
        
        float windowWidth = static_cast<float>(g_ctx->width());
        float windowHeight = static_cast<float>(g_ctx->height());
        
        // Calculate scaling factors to fit the reference grid to current window
        float scaleX = windowWidth / gridConfig_.referenceWidth;
        float scaleY = windowHeight / gridConfig_.referenceHeight;
        
        float cellWidth = (gridConfig_.referenceWidth / gridConfig_.cols) * scaleX;
        float cellHeight = (gridConfig_.referenceHeight / gridConfig_.rows) * scaleY;
        
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
        // Draw grid first (behind everything)
        drawGrid(cr);
        
        if (!g_ctx) return;
        
        // Calculate scaling factors to scale shapes from reference size to current window
        float windowWidth = static_cast<float>(g_ctx->width());
        float windowHeight = static_cast<float>(g_ctx->height());
        float scaleX = windowWidth / gridConfig_.referenceWidth;
        float scaleY = windowHeight / gridConfig_.referenceHeight;
        
        cairo_save(cr);
        // Apply scaling transformation to make shapes scale with window
        cairo_scale(cr, scaleX, scaleY);
        
        // Draw all shapes (they will be automatically scaled)
        for (auto &shape : shapes_) {
            shape->draw(cr);
        }
        
        cairo_restore(cr);
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
