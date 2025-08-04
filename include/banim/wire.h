#pragma once

#include "banim/animatable.h"
#include "banim/port_interface.h"
#include <string>
#include <vector>
#include <memory>

namespace banim {

class Wire : public Line {
public:
    // Simple API: Connect two port providers by port names
    Wire(std::shared_ptr<IPortProvider> fromProvider, const std::string& fromPortName,
         std::shared_ptr<IPortProvider> toProvider, const std::string& toPortName);
    
    // Simple API: Connect two port providers by port direction and index
    Wire(std::shared_ptr<IPortProvider> fromProvider, PortDirection fromDirection, int fromPortIndex,
         std::shared_ptr<IPortProvider> toProvider, PortDirection toDirection, int toPortIndex);
    
    // Update wire routing when blocks move
    void updateRouting();
    
    // Override draw to ensure routing is up to date
    void draw(cairo_t* cr) override;
    
    // Get the connected providers
    std::shared_ptr<IPortProvider> getFromProvider() const { return fromProvider_; }
    std::shared_ptr<IPortProvider> getToProvider() const { return toProvider_; }
    
    // Enable/disable automatic routing
    void setAutoRoute(bool enable) { autoRoute_ = enable; }
    bool isAutoRoute() const { return autoRoute_; }

private:
    std::shared_ptr<IPortProvider> fromProvider_;
    std::shared_ptr<IPortProvider> toProvider_;
    std::string fromPortName_;
    std::string toPortName_;
    PortDirection fromDirection_;
    PortDirection toDirection_;
    int fromPortIndex_;
    int toPortIndex_;
    bool usePortNames_;
    bool autoRoute_;
    
    // Track last known provider positions to detect movement
    GridCoord lastFromProviderPos_;
    GridCoord lastToProviderPos_;
    
    void calculateAutoRoute();
    std::vector<GridCoord> generatePathBetweenPorts(const Port* fromPort, const Port* toPort);
    Port* getFromPort();
    Port* getToPort();
    bool needsRoutingUpdate();
};

} // namespace banim
