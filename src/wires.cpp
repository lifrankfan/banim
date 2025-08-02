#include "banim/wires.h"
#include "banim/scene.h"
#include "banim/block.h"
#include <cmath>
#include <algorithm>

namespace banim {

// ────────────── WIRE ──────────────

// ELEGANT API: Create wire between named ports
Wire::Wire(std::shared_ptr<Block> fromBlock, const std::string& fromPortName,
           std::shared_ptr<Block> toBlock, const std::string& toPortName,
           float width)
    : wireWidth_(width), duration_(default_duration), autoRoute_(true),
      coordinateSystem_(CoordinateSystem::GRID), fromBlock_(fromBlock), toBlock_(toBlock),
      fromPortName_(fromPortName), toPortName_(toPortName), 
      usePortNames_(true), fromPortIndex_(0), toPortIndex_(0) {
    
    updateRouting();
}

// ELEGANT API: Create wire between indexed ports
Wire::Wire(std::shared_ptr<Block> fromBlock, PortDirection fromDirection, int fromPortIndex,
           std::shared_ptr<Block> toBlock, PortDirection toDirection, int toPortIndex,
           float width)
    : wireWidth_(width), duration_(default_duration), autoRoute_(true),
      coordinateSystem_(CoordinateSystem::GRID), fromBlock_(fromBlock), toBlock_(toBlock),
      fromDirection_(fromDirection), toDirection_(toDirection),
      fromPortIndex_(fromPortIndex), toPortIndex_(toPortIndex), usePortNames_(false) {
    
    updateRouting();
}

// Legacy API: Grid coordinates
Wire::Wire(const GridCoord& from, const GridCoord& to, float width, float duration)
    : wireWidth_(width), duration_(duration), autoRoute_(false),
      coordinateSystem_(CoordinateSystem::GRID), usePortNames_(false),
      fromPortIndex_(0), toPortIndex_(0) {
    
    gridWaypoints_ = {from, to};
    updatePixelWaypoints();
}

// Legacy API: Multiple waypoints
Wire::Wire(const std::vector<GridCoord>& waypoints, float width, float duration)
    : wireWidth_(width), duration_(duration), autoRoute_(false),
      coordinateSystem_(CoordinateSystem::GRID), usePortNames_(false),
      fromPortIndex_(0), toPortIndex_(0) {
    
    gridWaypoints_ = waypoints;
    updatePixelWaypoints();
}

void Wire::updateFromGrid(const Scene* scene) {
    if (coordinateSystem_ == CoordinateSystem::GRID && scene) {
        if (autoRoute_) {
            updateRouting();
        }
        
        // Convert grid waypoints to pixel coordinates for drawing
        pixelWaypoints_.clear();
        for (const auto& gridPoint : gridWaypoints_) {
            auto [pixelX, pixelY] = scene->gridToPixel(gridPoint);
            pixelWaypoints_.emplace_back(pixelX, pixelY);
        }
        
        // Set shape position to first waypoint for compatibility
        if (!pixelWaypoints_.empty()) {
            x_ = pixelWaypoints_[0].first;
            y_ = pixelWaypoints_[0].second;
        }
    }
}

void Wire::updateRouting() {
    if (autoRoute_ && fromBlock_ && toBlock_) {
        calculateAutoRoute();
    }
    updatePixelWaypoints();
}

void Wire::calculateAutoRoute() {
    Port* fromPort = getFromPort();
    Port* toPort = getToPort();
    
    if (!fromPort || !toPort) {
        // Fallback: just connect block centers if we don't have a scene reference
        // For now, just create a simple direct connection
        gridWaypoints_.clear();
        gridWaypoints_.push_back(GridCoord(0, 0));  // Will be updated when scene is available
        gridWaypoints_.push_back(GridCoord(1, 1));
        return;
    }
    
    gridWaypoints_ = generatePathBetweenPorts(fromPort, toPort);
}

std::vector<GridCoord> Wire::generatePathBetweenPorts(const Port* fromPort, const Port* toPort) {
    if (!fromPort || !toPort) return {};
    
    GridCoord start = fromPort->position;
    GridCoord end = toPort->position;
    
    // Simple L-shaped routing for now
    // TODO: Could be enhanced with more sophisticated algorithms
    std::vector<GridCoord> path;
    path.push_back(start);
    
    // Choose routing strategy based on port directions
    PortDirection fromDir = fromPort->direction;
    PortDirection toDir = toPort->direction;
    
    float midX, midY;
    
    // Create intermediate waypoints for clean routing
    if ((fromDir == PortDirection::RIGHT && toDir == PortDirection::LEFT) ||
        (fromDir == PortDirection::LEFT && toDir == PortDirection::RIGHT)) {
        // Horizontal routing
        midX = (start.x + end.x) / 2.0f;
        path.emplace_back(midX, start.y);
        path.emplace_back(midX, end.y);
    } else if ((fromDir == PortDirection::TOP && toDir == PortDirection::BOTTOM) ||
               (fromDir == PortDirection::BOTTOM && toDir == PortDirection::TOP)) {
        // Vertical routing
        midY = (start.y + end.y) / 2.0f;
        path.emplace_back(start.x, midY);
        path.emplace_back(end.x, midY);
    } else {
        // Mixed routing - create L-shape based on directions
        if (fromDir == PortDirection::RIGHT || fromDir == PortDirection::LEFT) {
            // Start horizontal, then vertical
            path.emplace_back(end.x, start.y);
        } else {
            // Start vertical, then horizontal
            path.emplace_back(start.x, end.y);
        }
    }
    
    path.push_back(end);
    return path;
}

Port* Wire::getFromPort() {
    if (!fromBlock_) return nullptr;
    
    if (usePortNames_) {
        // Search all directions for the named port
        for (auto dir : {PortDirection::LEFT, PortDirection::RIGHT, PortDirection::TOP, PortDirection::BOTTOM}) {
            if (auto* port = fromBlock_->getPort(dir, fromPortName_)) {
                return port;
            }
        }
    } else {
        return fromBlock_->getPort(fromDirection_, fromPortIndex_);
    }
    return nullptr;
}

Port* Wire::getToPort() {
    if (!toBlock_) return nullptr;
    
    if (usePortNames_) {
        // Search all directions for the named port
        for (auto dir : {PortDirection::LEFT, PortDirection::RIGHT, PortDirection::TOP, PortDirection::BOTTOM}) {
            if (auto* port = toBlock_->getPort(dir, toPortName_)) {
                return port;
            }
        }
    } else {
        return toBlock_->getPort(toDirection_, toPortIndex_);
    }
    return nullptr;
}

void Wire::updatePixelWaypoints() {
    pixelWaypoints_.clear();
    
    if (gridWaypoints_.empty()) return;
    
    // For grid-based wires, we need scene context for conversion
    // For now, store grid coordinates and convert when drawn
    for (const auto& gridPoint : gridWaypoints_) {
        // We'll convert during drawing when we have scene context
        pixelWaypoints_.emplace_back(gridPoint.x, gridPoint.y);
    }
    
    // Set shape position to first waypoint for compatibility
    if (!pixelWaypoints_.empty()) {
        x_ = pixelWaypoints_[0].first;
        y_ = pixelWaypoints_[0].second;
    }
}

void Wire::addWaypoint(const GridCoord& point) {
    autoRoute_ = false;  // Disable auto-routing when manually adding waypoints
    gridWaypoints_.push_back(point);
    updatePixelWaypoints();
}

void Wire::setWaypoints(const std::vector<GridCoord>& waypoints) {
    autoRoute_ = false;  // Disable auto-routing when manually setting waypoints
    gridWaypoints_ = waypoints;
    updatePixelWaypoints();
}

void Wire::updatePixelWaypoints(const std::vector<GridCoord>& gridWaypoints, const Scene* scene) {
    // Legacy method for backward compatibility
    pixelWaypoints_.clear();
    if (!scene) return;
    
    for (const auto& gridPoint : gridWaypoints) {
        auto [pixelX, pixelY] = scene->gridToPixel(gridPoint);
        pixelWaypoints_.emplace_back(pixelX, pixelY);
    }
    
    // Set shape position to first waypoint for compatibility
    if (!pixelWaypoints_.empty()) {
        x_ = pixelWaypoints_[0].first;
        y_ = pixelWaypoints_[0].second;
    }
}

void Wire::draw(cairo_t* cr) {
    if (pixelWaypoints_.size() < 2) return;
    
    cairo_save(cr);
    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    cairo_set_line_width(cr, wireWidth_);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
    
    // Move to first point
    cairo_move_to(cr, pixelWaypoints_[0].first, pixelWaypoints_[0].second);
    
    // Draw lines to subsequent points
    for (size_t i = 1; i < pixelWaypoints_.size(); ++i) {
        cairo_line_to(cr, pixelWaypoints_[i].first, pixelWaypoints_[i].second);
    }
    
    cairo_stroke(cr);
    cairo_restore(cr);
}

// ────────────── ARROW ──────────────

Arrow::Arrow(const GridCoord& from, const GridCoord& to, const Scene* scene,
             float headSize, float width, float duration)
    : arrowHeadSize_(headSize), arrowWidth_(width), duration_(duration) {
    
    if (scene) {
        auto [fromPixelX, fromPixelY] = scene->gridToPixel(from);
        auto [toPixelX, toPixelY] = scene->gridToPixel(to);
        fromX_ = fromPixelX;
        fromY_ = fromPixelY;
        toX_ = toPixelX;
        toY_ = toPixelY;
        
        // Convert grid-based head size to pixel size
        auto [cellWidth, cellHeight] = scene->getGridCellSize();
        arrowHeadSize_ = headSize * std::min(cellWidth, cellHeight);
    } else {
        fromX_ = from.x;
        fromY_ = from.y;
        toX_ = to.x;
        toY_ = to.y;
    }
    
    // Set shape position to the "from" point
    x_ = fromX_;
    y_ = fromY_;
}

void Arrow::draw(cairo_t* cr) {
    cairo_save(cr);
    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    cairo_set_line_width(cr, arrowWidth_);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    
    // Draw the main line
    cairo_move_to(cr, fromX_, fromY_);
    cairo_line_to(cr, toX_, toY_);
    cairo_stroke(cr);
    
    // Calculate arrow head
    float angle = std::atan2(toY_ - fromY_, toX_ - fromX_);
    float headLength = arrowHeadSize_;
    float headAngle = M_PI / 6;  // 30 degrees
    
    // Arrow head points
    float head1X = toX_ - headLength * std::cos(angle - headAngle);
    float head1Y = toY_ - headLength * std::sin(angle - headAngle);
    float head2X = toX_ - headLength * std::cos(angle + headAngle);
    float head2Y = toY_ - headLength * std::sin(angle + headAngle);
    
    // Draw arrow head
    cairo_move_to(cr, toX_, toY_);
    cairo_line_to(cr, head1X, head1Y);
    cairo_move_to(cr, toX_, toY_);
    cairo_line_to(cr, head2X, head2Y);
    cairo_stroke(cr);
    
    cairo_restore(cr);
}

} // namespace banim
