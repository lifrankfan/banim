#pragma once

#include <cairo/cairo.h>
#include <memory>
#include <vector>
#include <queue>
#include <variant>
#include "banim/grid.h"

namespace banim {

class Shape;
class Animation;

struct AddAction {
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Animation> spawnAnimation; // nullptr for no animation
};

using TimelineAction = std::variant<std::shared_ptr<Animation>, AddAction>;

// Grid configuration for a scene
struct GridConfig {
    int cols = 16;          // Number of grid columns
    int rows = 9;           // Number of grid rows  
    bool displayGrid = false; // Whether to show grid lines
    float lineWidth = 1.0f; // Grid line width
    float r = 0.8f, g = 0.8f, b = 0.8f, a = 0.3f; // Grid line color
    
    // Fixed reference size for consistent grid scaling
    float referenceWidth = 800.0f;
    float referenceHeight = 600.0f;
    
    GridConfig() = default;
    GridConfig(int cols, int rows, bool display = false) 
        : cols(cols), rows(rows), displayGrid(display) {}
    GridConfig(int cols, int rows, float refWidth, float refHeight, bool display = false)
        : cols(cols), rows(rows), referenceWidth(refWidth), referenceHeight(refHeight), displayGrid(display) {}
};

class Scene {
  public:
    Scene() = default;
    Scene(const GridConfig& gridConfig) : gridConfig_(gridConfig) {}
    
    // Grid management
    void setGridConfig(const GridConfig& config) { gridConfig_ = config; }
    const GridConfig& getGridConfig() const { return gridConfig_; }
    void displayGrid(bool show) { gridConfig_.displayGrid = show; }
    bool isGridDisplayed() const { return gridConfig_.displayGrid; }
    
    // Convert grid coordinates to pixel coordinates
    std::pair<float, float> gridToPixel(const GridCoord& coord) const;
    std::pair<float, float> gridToPixel(float gridX, float gridY) const;
    
    // Convert pixel coordinates to grid coordinates
    GridCoord pixelToGrid(float pixelX, float pixelY) const;
    
    // Get grid cell size in pixels
    std::pair<float, float> getGridCellSize() const;
    
    // Existing methods
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
    GridConfig gridConfig_;
    
    void drawGrid(cairo_t *cr) const;
};


} // namespace banim