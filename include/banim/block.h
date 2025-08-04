#pragma once

#include "banim/animatable.h"
#include <string>
#include <vector>

namespace banim {

enum class PortDirection {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

struct Port {
    PortDirection direction;
    std::string name;
    GridCoord position;  // Grid-based position
    
    Port(PortDirection dir, const std::string& portName) 
        : direction(dir), name(portName) {}
};

class Block : public Rectangle {
public:
    // Grid-based constructor
    Block(const GridCoord& position, float gridWidth, float gridHeight, 
          const std::string& label = "");
    
    void draw(cairo_t* cr) override;
    
    // Port management
    void addPort(PortDirection direction, const std::string& name);
    void removePort(PortDirection direction, const std::string& name);
    void clearPorts(PortDirection direction);
    void clearAllPorts();
    
    std::vector<Port>& getPorts(PortDirection direction);
    const std::vector<Port>& getPorts(PortDirection direction) const;
    Port* getPort(PortDirection direction, const std::string& name);
    Port* getPort(PortDirection direction, int index);
    
    // Label management
    void setLabel(const std::string& label);
    const std::string& getLabel() const { return label_; }
    void setLabelColor(float r, float g, float b, float a = 1.0f);
    void setLabelSize(float size) { labelSize_ = size; }
    
    // Grid size access for wire connections
    float getGridWidth() const { return gridSize_.x; }
    float getGridHeight() const { return gridSize_.y; }
    
    // Override from Rectangle to update ports when size changes
    void setAnimatableSize(float w, float h) override {
        Rectangle::setAnimatableSize(w, h);
        updatePortPositions();
    }
    
    // Override position changes to update ports
    void setGridPos(const GridCoord& pos) override {
        Rectangle::setGridPos(pos);
        updatePortPositions();
    }
    void setGridPos(float x, float y) override {
        Rectangle::setGridPos(x, y);
        updatePortPositions();
    }

private:
    std::string label_;
    float labelSize_;
    float labelR_, labelG_, labelB_, labelA_;
    
    std::vector<Port> leftPorts_;
    std::vector<Port> rightPorts_;
    std::vector<Port> topPorts_;
    std::vector<Port> bottomPorts_;
    
    void updatePortPositions();
    void updatePortsForDirection(std::vector<Port>& ports, PortDirection direction);
    std::vector<Port>& getPortVector(PortDirection direction);
    const std::vector<Port>& getPortVector(PortDirection direction) const;
};

} // namespace banim
