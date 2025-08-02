#include "banim/wires.h"
#include "banim/scene.h"
#include <cmath>

namespace banim {

// ────────────── WIRE ──────────────

Wire::Wire(const GridCoord& from, const GridCoord& to, const Scene* scene, 
           float width, float duration)
    : wireWidth_(width), duration_(duration) {
    std::vector<GridCoord> waypoints = {from, to};
    updatePixelWaypoints(waypoints, scene);
}

Wire::Wire(const std::vector<GridCoord>& waypoints, const Scene* scene,
           float width, float duration)
    : wireWidth_(width), duration_(duration) {
    updatePixelWaypoints(waypoints, scene);
}

void Wire::updatePixelWaypoints(const std::vector<GridCoord>& gridWaypoints, const Scene* scene) {
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

void Wire::addWaypoint(const GridCoord& point, const Scene* scene) {
    if (!scene) return;
    auto [pixelX, pixelY] = scene->gridToPixel(point);
    pixelWaypoints_.emplace_back(pixelX, pixelY);
}

void Wire::setWaypoints(const std::vector<GridCoord>& waypoints, const Scene* scene) {
    updatePixelWaypoints(waypoints, scene);
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
