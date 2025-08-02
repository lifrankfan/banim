#pragma once
#include "banim/shapes.h"
#include "banim/grid.h"
#include "banim/block.h"
#include <vector>
#include <memory>

#define default_duration 0.5f

namespace banim {

// Forward declaration
class Block;
struct Port;

// Wire class for connecting components in architecture diagrams
class Wire : public Shape {
public:
    // API: Create a wire between two ports on blocks
    Wire(std::shared_ptr<Block> fromBlock, const std::string& fromPortName,
         std::shared_ptr<Block> toBlock, const std::string& toPortName,
         float width = 2.0f);
    
    // Alternative: Use port indices instead of names
    Wire(std::shared_ptr<Block> fromBlock, PortDirection fromDirection, int fromPortIndex,
         std::shared_ptr<Block> toBlock, PortDirection toDirection, int toPortIndex,
         float width = 2.0f);
    
    // Legacy API: Create a wire from one grid position to another
    Wire(const GridCoord& from, const GridCoord& to, 
         float width = 2.0f, float duration = default_duration);
    
    // Legacy API: Create a wire with multiple waypoints for complex routing
    Wire(const std::vector<GridCoord>& waypoints,
         float width = 2.0f, float duration = default_duration);

    void draw(cairo_t* cr) override;
    void setSize(float w, float h) override {}  // Wires don't use traditional size
    
    // Wire-specific methods
    void setWireWidth(float width) { wireWidth_ = width; }
    float getWireWidth() const { return wireWidth_; }
    
    // Auto-routing options
    void setAutoRoute(bool enable) { autoRoute_ = enable; updateRouting(); }
    bool getAutoRoute() const { return autoRoute_; }
    
    // Manual waypoint control (disables auto-routing)
    void addWaypoint(const GridCoord& point);
    void setWaypoints(const std::vector<GridCoord>& waypoints);
    
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
    
    // Grid system integration
    void updateFromGrid(const Scene* scene) override;

private:
    std::vector<std::pair<float, float>> pixelWaypoints_;  // Converted to pixel coordinates
    std::vector<GridCoord> gridWaypoints_;                 // Grid coordinates for waypoints
    float wireWidth_;
    float duration_;
    bool autoRoute_;
    CoordinateSystem coordinateSystem_;
    
    // Port-based connection data
    std::shared_ptr<Block> fromBlock_;
    std::shared_ptr<Block> toBlock_;
    std::string fromPortName_;
    std::string toPortName_;
    PortDirection fromDirection_;
    PortDirection toDirection_;
    int fromPortIndex_;
    int toPortIndex_;
    bool usePortNames_;  // true if using port names, false if using indices
    
    // Routing methods
    void updateRouting();
    void calculateAutoRoute();
    std::vector<GridCoord> generatePathBetweenPorts(const Port* fromPort, const Port* toPort);
    void updatePixelWaypoints();
    void updatePixelWaypoints(const std::vector<GridCoord>& gridWaypoints, const Scene* scene);  // Legacy method
    
    // Port access helpers
    Port* getFromPort();
    Port* getToPort();
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
