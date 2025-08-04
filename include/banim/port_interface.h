#pragma once

#include "banim/grid.h"
#include <string>
#include <vector>
#include <memory>

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

// Common interface for objects that have ports (blocks, logic gates, etc.)
class IPortProvider {
public:
    virtual ~IPortProvider() = default;
    
    // Port management
    virtual void addPort(PortDirection direction, const std::string& name) = 0;
    virtual void removePort(PortDirection direction, const std::string& name) = 0;
    virtual void clearPorts(PortDirection direction) = 0;
    virtual void clearAllPorts() = 0;
    
    virtual std::vector<Port>& getPorts(PortDirection direction) = 0;
    virtual const std::vector<Port>& getPorts(PortDirection direction) const = 0;
    virtual Port* getPort(PortDirection direction, const std::string& name) = 0;
    virtual Port* getPort(PortDirection direction, int index) = 0;
    
    // Grid size access for wire connections
    virtual float getGridWidth() const = 0;
    virtual float getGridHeight() const = 0;
    
    // Position access
    virtual GridCoord getGridPos() const = 0;
};

} // namespace banim
