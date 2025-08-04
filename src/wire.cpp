#include "banim/wire.h"
#include "banim/scene.h"
#include "banim/init.h"
#include <cmath>
#include <algorithm>

namespace banim {

// Simple API: Connect two port providers by port names
Wire::Wire(std::shared_ptr<IPortProvider> fromProvider, const std::string& fromPortName,
           std::shared_ptr<IPortProvider> toProvider, const std::string& toPortName)
    : Line(GridCoord(0, 0), GridCoord(1, 1)), // Temporary coordinates, will be updated
      fromProvider_(fromProvider), toProvider_(toProvider),
      fromPortName_(fromPortName), toPortName_(toPortName),
      usePortNames_(true), fromPortIndex_(0), toPortIndex_(0), autoRoute_(false) {
    
    // Set wire appearance
    setColor(0.2f, 0.6f, 1.0f, 1.0f); // Blue wire
    setStrokeWidth(2.0f);
    
    // Store initial provider positions
    lastFromProviderPos_ = fromProvider_->getGridPos();
    lastToProviderPos_ = toProvider_->getGridPos();
    
    updateRouting();
}

// Simple API: Connect two port providers by port direction and index
Wire::Wire(std::shared_ptr<IPortProvider> fromProvider, PortDirection fromDirection, int fromPortIndex,
           std::shared_ptr<IPortProvider> toProvider, PortDirection toDirection, int toPortIndex)
    : Line(GridCoord(0, 0), GridCoord(1, 1)), // Temporary coordinates, will be updated
      fromProvider_(fromProvider), toProvider_(toProvider),
      fromDirection_(fromDirection), toDirection_(toDirection),
      fromPortIndex_(fromPortIndex), toPortIndex_(toPortIndex),
      usePortNames_(false), autoRoute_(false) {
    
    // Set wire appearance
    setColor(0.2f, 0.6f, 1.0f, 1.0f); // Blue wire
    setStrokeWidth(2.0f);
    
    // Store initial provider positions
    lastFromProviderPos_ = fromProvider_->getGridPos();
    lastToProviderPos_ = toProvider_->getGridPos();
    
    updateRouting();
}

void Wire::updateRouting() {
    if (!fromProvider_ || !toProvider_) return;
    
    // Always update endpoints to track port positions
    Port* fromPort = getFromPort();
    Port* toPort = getToPort();
    
    if (fromPort && toPort) {
        // Update start and end positions to follow ports
        setGridPos(fromPort->position);
        setEndPos(toPort->position);
    }
    
    // Only auto-generate waypoints if autoRoute is enabled
    if (autoRoute_) {
        calculateAutoRoute();
    }
    // If autoRoute is false, preserve existing waypoints but update endpoints
    
    // Update stored positions
    lastFromProviderPos_ = fromProvider_->getGridPos();
    lastToProviderPos_ = toProvider_->getGridPos();
}

void Wire::draw(cairo_t* cr) {
    // Check if blocks have moved and update routing if needed
    if (needsRoutingUpdate()) {
        updateRouting();
    }
    
    // Custom drawing for precise port positioning (no cell-centering offset)
    extern GLContext* g_ctx;
    if (!g_ctx) return;
    
    extern Scene *g_currentScene;
    if (!g_currentScene) return;
    
    // Convert grid coordinates to pixel coordinates without cell-centering offset
    float windowWidth = static_cast<float>(g_ctx->width());
    float windowHeight = static_cast<float>(g_ctx->height());
    
    const GridConfig& gridConfig = g_currentScene->getGridConfig();
    float cellWidth = windowWidth / static_cast<float>(gridConfig.cols);
    float cellHeight = windowHeight / static_cast<float>(gridConfig.rows);
    
    cairo_save(cr);
    
    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    cairo_set_line_width(cr, strokeWidth_);
    
    // Start from the start position (no +0.5 offset for precise port positioning)
    float currentX = gridPos_.x * cellWidth;
    float currentY = gridPos_.y * cellHeight;
    cairo_move_to(cr, currentX, currentY);
    
    // Draw through all waypoints (no +0.5 offset)
    for (int i = 0; i < getWaypointCount(); ++i) {
        GridCoord waypoint = getWaypoint(i);
        float waypointX = waypoint.x * cellWidth;
        float waypointY = waypoint.y * cellHeight;
        cairo_line_to(cr, waypointX, waypointY);
    }
    
    // End at the end position (no +0.5 offset)
    GridCoord endPos = getEndPos();
    float endPixelX = endPos.x * cellWidth;
    float endPixelY = endPos.y * cellHeight;
    cairo_line_to(cr, endPixelX, endPixelY);
    
    cairo_stroke(cr);
    
    cairo_restore(cr);
}

bool Wire::needsRoutingUpdate() {
    if (!fromProvider_ || !toProvider_) return false;
    
    // Check if blocks have moved
    return (lastFromProviderPos_.x != fromProvider_->getGridPos().x ||
            lastFromProviderPos_.y != fromProvider_->getGridPos().y ||
            lastToProviderPos_.x != toProvider_->getGridPos().x ||
            lastToProviderPos_.y != toProvider_->getGridPos().y);
}

void Wire::calculateAutoRoute() {
    Port* fromPort = getFromPort();
    Port* toPort = getToPort();
    
    if (!fromPort || !toPort) {
        // Fallback: connect provider centers
        GridCoord fromCenter = fromProvider_->getGridPos();
        fromCenter.x += fromProvider_->getGridWidth() * 0.5f;
        fromCenter.y += fromProvider_->getGridHeight() * 0.5f;
        
        GridCoord toCenter = toProvider_->getGridPos();
        toCenter.x += toProvider_->getGridWidth() * 0.5f;
        toCenter.y += toProvider_->getGridHeight() * 0.5f;
        
        // Update line endpoints
        setGridPos(fromCenter);
        setEndPos(toCenter);
        clearWaypoints();
        return;
    }
    
    // Generate path between ports
    std::vector<GridCoord> path = generatePathBetweenPorts(fromPort, toPort);
    
    if (path.size() >= 2) {
        // Set start and end points
        setGridPos(path[0]);
        setEndPos(path.back());
        
        // Clear existing waypoints and add intermediate points
        clearWaypoints();
        for (size_t i = 1; i < path.size() - 1; ++i) {
            addWaypoint(path[i]);
        }
    }
}

std::vector<GridCoord> Wire::generatePathBetweenPorts(const Port* fromPort, const Port* toPort) {
    if (!fromPort || !toPort) return {};
    
    GridCoord start = fromPort->position;
    GridCoord end = toPort->position;
    
    std::vector<GridCoord> path;
    path.push_back(start);
    
    // Get port directions for smart routing
    PortDirection fromDir = fromPort->direction;
    PortDirection toDir = toPort->direction;
    
    // Create offset points away from the blocks for cleaner routing
    GridCoord startOffset = start;
    GridCoord endOffset = end;
    
    float offsetDistance = 0.5f; // Half a grid cell
    
    // Offset from start port
    switch (fromDir) {
        case PortDirection::LEFT:
            startOffset.x -= offsetDistance;
            break;
        case PortDirection::RIGHT:
            startOffset.x += offsetDistance;
            break;
        case PortDirection::TOP:
            startOffset.y -= offsetDistance;
            break;
        case PortDirection::BOTTOM:
            startOffset.y += offsetDistance;
            break;
    }
    
    // Offset from end port
    switch (toDir) {
        case PortDirection::LEFT:
            endOffset.x -= offsetDistance;
            break;
        case PortDirection::RIGHT:
            endOffset.x += offsetDistance;
            break;
        case PortDirection::TOP:
            endOffset.y -= offsetDistance;
            break;
        case PortDirection::BOTTOM:
            endOffset.y += offsetDistance;
            break;
    }
    
    // Add offset points if they're different from the original ports
    if (startOffset.x != start.x || startOffset.y != start.y) {
        path.push_back(startOffset);
    }
    
    // Choose routing strategy based on port directions
    if ((fromDir == PortDirection::RIGHT && toDir == PortDirection::LEFT) ||
        (fromDir == PortDirection::LEFT && toDir == PortDirection::RIGHT)) {
        // Horizontal routing
        float midX = (startOffset.x + endOffset.x) * 0.5f;
        path.emplace_back(midX, startOffset.y);
        path.emplace_back(midX, endOffset.y);
    } else if ((fromDir == PortDirection::TOP && toDir == PortDirection::BOTTOM) ||
               (fromDir == PortDirection::BOTTOM && toDir == PortDirection::TOP)) {
        // Vertical routing
        float midY = (startOffset.y + endOffset.y) * 0.5f;
        path.emplace_back(startOffset.x, midY);
        path.emplace_back(endOffset.x, midY);
    } else {
        // Mixed routing - create L-shape based on directions
        if (fromDir == PortDirection::RIGHT || fromDir == PortDirection::LEFT) {
            // Start horizontal, then vertical
            path.emplace_back(endOffset.x, startOffset.y);
        } else {
            // Start vertical, then horizontal
            path.emplace_back(startOffset.x, endOffset.y);
        }
    }
    
    // Add end offset point if different from end
    if (endOffset.x != end.x || endOffset.y != end.y) {
        path.push_back(endOffset);
    }
    
    path.push_back(end);
    return path;
}

Port* Wire::getFromPort() {
    if (!fromProvider_) return nullptr;
    
    if (usePortNames_) {
        // Search all directions for the named port
        for (auto dir : {PortDirection::LEFT, PortDirection::RIGHT, PortDirection::TOP, PortDirection::BOTTOM}) {
            if (auto* port = fromProvider_->getPort(dir, fromPortName_)) {
                return port;
            }
        }
    } else {
        return fromProvider_->getPort(fromDirection_, fromPortIndex_);
    }
    return nullptr;
}

Port* Wire::getToPort() {
    if (!toProvider_) return nullptr;
    
    if (usePortNames_) {
        // Search all directions for the named port
        for (auto dir : {PortDirection::LEFT, PortDirection::RIGHT, PortDirection::TOP, PortDirection::BOTTOM}) {
            if (auto* port = toProvider_->getPort(dir, toPortName_)) {
                return port;
            }
        }
    } else {
        return toProvider_->getPort(toDirection_, toPortIndex_);
    }
    return nullptr;
}

} // namespace banim
