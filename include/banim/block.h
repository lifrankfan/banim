#pragma once
#include "banim/shapes.h"
#include "banim/grid.h"
#include "banim/text.h"
#include <vector>
#include <string>
#include <memory>

namespace banim {

// Port direction enum
enum class PortDirection {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

// Port structure for tracking connection points
struct Port {
    PortDirection direction;
    std::string name;           // Optional name for the port
    GridCoord position;         // Grid position of the port
    float pixelX, pixelY;       // Pixel position for wire connections
    
    Port(PortDirection dir, const std::string& portName = "") 
        : direction(dir), name(portName), position(0, 0), pixelX(0), pixelY(0) {}
};

// Generic block class for architecture diagrams
class Block : public Rectangle {
public:
    // Create a block at grid position with specified size
    Block(const GridCoord& position, float gridWidth, float gridHeight, 
          const std::string& label, const Scene* scene);
    
    // Create a block with pixel coordinates (for backward compatibility)
    Block(float x, float y, float width, float height, const std::string& label);

    void draw(cairo_t* cr) override;
    
    // Port management
    void addPort(PortDirection direction, const std::string& name = "");
    void removePort(PortDirection direction, const std::string& name = "");
    void clearPorts(PortDirection direction);
    void clearAllPorts();
    
    // Get port information for wire connections
    std::vector<Port>& getPorts(PortDirection direction);
    const std::vector<Port>& getPorts(PortDirection direction) const;
    Port* getPort(PortDirection direction, const std::string& name);
    Port* getPort(PortDirection direction, int index = 0);  // Get port by index
    
    // Block appearance
    void setLabel(const std::string& label);
    const std::string& getLabel() const { return label_; }
    void setLabelSize(float size) { labelSize_ = size; }
    void setLabelColor(float r, float g, float b, float a = 1.0f);
    
    // Grid integration
    void setScene(const Scene* scene) { scene_ = scene; updatePortPositions(); }
    
    // Override setSize to update port positions
    void setSize(float w, float h) override;
    
    // Animation support
    void getAnimatableSize(float& w, float& h) const override {
        w = getWidth();
        h = getHeight();
    }
    
    void setAnimatableSize(float w, float h) override {
        setSize(w, h);
    }
    
    void resetForAnimation() override {
        // Blocks don't need special animation reset
    }

private:
    std::string label_;
    float labelSize_;
    float labelR_, labelG_, labelB_, labelA_;
    
    // Port storage - separate vectors for each direction
    std::vector<Port> leftPorts_;
    std::vector<Port> rightPorts_;
    std::vector<Port> topPorts_;
    std::vector<Port> bottomPorts_;
    
    const Scene* scene_;  // For grid coordinate conversion
    bool useGridSystem_;  // Track if this block uses grid coordinates
    
    // Internal methods
    void updatePortPositions();
    void updatePortsForDirection(std::vector<Port>& ports, PortDirection direction);
    std::vector<Port>& getPortVector(PortDirection direction);
    const std::vector<Port>& getPortVector(PortDirection direction) const;
};

} // namespace banim
