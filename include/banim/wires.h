#pragma once
#include "banim/shapes.h"
#include "banim/grid.h"
#include <vector>

#define default_duration 0.5f

namespace banim {

// Wire class for connecting components in architecture diagrams
class Wire : public Shape {
public:
    // Create a wire from one grid position to another
    Wire(const GridCoord& from, const GridCoord& to, const Scene* scene, 
         float width = 2.0f, float duration = default_duration);
    
    // Create a wire with multiple waypoints for complex routing
    Wire(const std::vector<GridCoord>& waypoints, const Scene* scene,
         float width = 2.0f, float duration = default_duration);

    void draw(cairo_t* cr) override;
    void setSize(float w, float h) override {}  // Wires don't use traditional size
    
    // Wire-specific methods
    void setWireWidth(float width) { wireWidth_ = width; }
    float getWireWidth() const { return wireWidth_; }
    
    void addWaypoint(const GridCoord& point, const Scene* scene);
    void setWaypoints(const std::vector<GridCoord>& waypoints, const Scene* scene);
    
    // Animation support
    void getAnimatableSize(float& w, float& h) const override {
        w = wireWidth_;
        h = wireWidth_;
    }
    
    void setAnimatableSize(float w, float h) override {
        setWireWidth(w);
    }
    
    void resetForAnimation() override {
        // Wires don't need special animation reset
    }

private:
    std::vector<std::pair<float, float>> pixelWaypoints_;  // Converted to pixel coordinates
    float wireWidth_;
    float duration_;
    
    void updatePixelWaypoints(const std::vector<GridCoord>& gridWaypoints, const Scene* scene);
};

// Arrow class for showing data flow direction
class Arrow : public Shape {
public:
    Arrow(const GridCoord& from, const GridCoord& to, const Scene* scene,
          float headSize = 0.3f, float width = 2.0f, float duration = default_duration);

    void draw(cairo_t* cr) override;
    void setSize(float w, float h) override {}
    
    void setArrowHeadSize(float size) { arrowHeadSize_ = size; }
    float getArrowHeadSize() const { return arrowHeadSize_; }
    
    void getAnimatableSize(float& w, float& h) const override {
        w = arrowWidth_;
        h = arrowHeadSize_;
    }
    
    void setAnimatableSize(float w, float h) override {
        arrowWidth_ = w;
        arrowHeadSize_ = h;
    }
    
    void resetForAnimation() override {}

private:
    float fromX_, fromY_;
    float toX_, toY_;
    float arrowHeadSize_;
    float arrowWidth_;
    float duration_;
};

} // namespace banim
