#pragma once

#include "banim/animatable.h"
#include "banim/block.h"
#include <string>
#include <vector>
#include <memory>

namespace banim {

class Wire : public Line {
public:
    // Simple API: Connect two blocks by port names
    Wire(std::shared_ptr<Block> fromBlock, const std::string& fromPortName,
         std::shared_ptr<Block> toBlock, const std::string& toPortName);
    
    // Simple API: Connect two blocks by port direction and index
    Wire(std::shared_ptr<Block> fromBlock, PortDirection fromDirection, int fromPortIndex,
         std::shared_ptr<Block> toBlock, PortDirection toDirection, int toPortIndex);
    
    // Update wire routing when blocks move
    void updateRouting();
    
    // Override draw to ensure routing is up to date
    void draw(cairo_t* cr) override;
    
    // Get the connected blocks
    std::shared_ptr<Block> getFromBlock() const { return fromBlock_; }
    std::shared_ptr<Block> getToBlock() const { return toBlock_; }
    
    // Enable/disable automatic routing
    void setAutoRoute(bool enable) { autoRoute_ = enable; }
    bool isAutoRoute() const { return autoRoute_; }

private:
    std::shared_ptr<Block> fromBlock_;
    std::shared_ptr<Block> toBlock_;
    std::string fromPortName_;
    std::string toPortName_;
    PortDirection fromDirection_;
    PortDirection toDirection_;
    int fromPortIndex_;
    int toPortIndex_;
    bool usePortNames_;
    bool autoRoute_;
    
    // Track last known block positions to detect movement
    GridCoord lastFromBlockPos_;
    GridCoord lastToBlockPos_;
    
    void calculateAutoRoute();
    std::vector<GridCoord> generatePathBetweenPorts(const Port* fromPort, const Port* toPort);
    Port* getFromPort();
    Port* getToPort();
    bool needsRoutingUpdate();
};

} // namespace banim
