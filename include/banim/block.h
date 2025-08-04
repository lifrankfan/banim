#pragma once

#include "banim/animatable.h"
#include "banim/port_interface.h"
#include <string>
#include <vector>

namespace banim {

class Block : public Rectangle, public IPortProvider {
public:
    // Grid-based constructor
    Block(const GridCoord& position, float gridWidth, float gridHeight, 
          const std::string& label = "");
    
    void draw(cairo_t* cr) override;
    
    // IPortProvider implementation
    void addPort(PortDirection direction, const std::string& name) override;
    void removePort(PortDirection direction, const std::string& name) override;
    void clearPorts(PortDirection direction) override;
    void clearAllPorts() override;
    
    std::vector<Port>& getPorts(PortDirection direction) override;
    const std::vector<Port>& getPorts(PortDirection direction) const override;
    Port* getPort(PortDirection direction, const std::string& name) override;
    Port* getPort(PortDirection direction, int index) override;
    
    float getGridWidth() const override { return gridSize_.x; }
    float getGridHeight() const override { return gridSize_.y; }
    GridCoord getGridPos() const override { return gridPos_; }
    
    // Label management
    void setLabel(const std::string& label);
    const std::string& getLabel() const { return label_; }
    void setLabelColor(float r, float g, float b, float a = 1.0f);
    void setLabelSize(float size) { labelSize_ = size; }
    
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
    
    virtual void updatePortPositions();
    virtual void updatePortsForDirection(std::vector<Port>& ports, PortDirection direction);
    std::vector<Port>& getPortVector(PortDirection direction);
    const std::vector<Port>& getPortVector(PortDirection direction) const;
};

} // namespace banim
