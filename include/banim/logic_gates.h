#pragma once

#include "banim/animatable.h"
#include "banim/port_interface.h"
#include <string>
#include <vector>

namespace banim {

enum class GateType {
    AND,
    OR,
    XOR,
    NOT,
    NAND,
    NOR,
    XNOR
};

class LogicGate : public Rectangle, public IPortProvider {
public:
    // Create a logic gate facing a specific direction
    LogicGate(GateType type, PortDirection facing, const GridCoord& position, 
              float gridWidth = 1.0f, float gridHeight = 1.0f);
    
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
    
    // Animatable implementation
    void getAnimatableSize(float& w, float& h) const override {
        w = gridSize_.x; h = gridSize_.y;
    }
    void setAnimatableSize(float w, float h) override {
        gridSize_.x = w; gridSize_.y = h; updatePortPositions();
    }
    void resetForAnimation() override { /* nothing special needed */ }
    
    // Override position changes to update ports
    void setGridPos(const GridCoord& pos) override {
        gridPos_ = pos; updatePortPositions();
    }
    void setGridPos(float x, float y) override {
        gridPos_.x = x; gridPos_.y = y; updatePortPositions();
    }
    
    // Gate properties
    GateType getGateType() const { return gateType_; }
    PortDirection getFacing() const { return facing_; }
    void setFacing(PortDirection facing);
    
    // Set gate colors
    void setGateColor(float r, float g, float b, float a = 1.0f);
    void setSymbolColor(float r, float g, float b, float a = 1.0f);
    LogicGate& setFilled(bool filled) override { filled_ = filled; return *this; }

private:
    GateType gateType_;
    PortDirection facing_;
    
    // Colors for gate drawing
    float gateR_ = 0.9f, gateG_ = 0.9f, gateB_ = 0.9f, gateA_ = 1.0f;
    float symbolR_ = 0.0f, symbolG_ = 0.0f, symbolB_ = 0.0f, symbolA_ = 1.0f;
    bool filled_ = true;
    
    // Port storage
    std::vector<Port> leftPorts_;
    std::vector<Port> rightPorts_;
    std::vector<Port> topPorts_;
    std::vector<Port> bottomPorts_;
    
    void setupPorts();
    void updatePortPositions();
    void updatePortsForDirection(std::vector<Port>& ports, PortDirection direction);
    std::vector<Port>& getPortVector(PortDirection direction);
    const std::vector<Port>& getPortVector(PortDirection direction) const;
    
    // Individual gate shape drawing methods
    void drawAndGate(cairo_t* cr, float width, float height);
    void drawOrGate(cairo_t* cr, float width, float height);
    void drawXorGate(cairo_t* cr, float width, float height);
    void drawNotGate(cairo_t* cr, float width, float height);
    void drawInversionBubble(cairo_t* cr, float width, float height);
    
    std::string getGateSymbol() const;
};

} // namespace banim
