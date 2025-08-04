#include "banim/animatable.h"
#include "banim/scene.h"
#include "banim/animations.h"
#include "banim/init.h"
#include <cmath>
#include <memory>

namespace banim {

// ────────────── RECTANGLE ──────────────

Rectangle::Rectangle(const GridCoord& gridPos, float gridWidth, float gridHeight,
                     float duration, float r, float g, float b, float a, float rotation) 
    : duration_(duration) {
    gridPos_ = gridPos;
    gridSize_ = {gridWidth, gridHeight};
    r_ = r; g_ = g; b_ = b; a_ = a;
    rotation_ = rotation;
}

Rectangle& Rectangle::setBorderRadius(float radius) {
    borderRadius_ = radius;
    return *this;
}

void Rectangle::draw(cairo_t* cr) {
    if (!g_ctx) return;
    
    extern Scene *g_currentScene;
    if (!g_currentScene) return;
    
    // Convert grid coordinates to pixel coordinates
    float windowWidth = static_cast<float>(g_ctx->width());
    float windowHeight = static_cast<float>(g_ctx->height());
    
    const GridConfig& gridConfig = g_currentScene->getGridConfig();
    float cellWidth = windowWidth / static_cast<float>(gridConfig.cols);
    float cellHeight = windowHeight / static_cast<float>(gridConfig.rows);
    
    float pixelX = gridPos_.x * cellWidth;
    float pixelY = gridPos_.y * cellHeight;
    float pixelW = gridSize_.x * cellWidth;
    float pixelH = gridSize_.y * cellHeight;
    
    cairo_save(cr);
    cairo_translate(cr, pixelX + pixelW / 2, pixelY + pixelH / 2);
    cairo_rotate(cr, rotation_);
    cairo_translate(cr, -pixelW / 2, -pixelH / 2);

    if (borderRadius_ > 0.0f) {
        float r = std::min(borderRadius_, std::min(pixelW / 2, pixelH / 2));
        cairo_new_sub_path(cr);
        cairo_arc(cr, pixelW - r, r, r, -M_PI_2, 0);
        cairo_arc(cr, pixelW - r, pixelH - r, r, 0, M_PI_2);
        cairo_arc(cr, r, pixelH - r, r, M_PI_2, M_PI);
        cairo_arc(cr, r, r, r, M_PI, 3 * M_PI_2);
        cairo_close_path(cr);
    } else {
        cairo_rectangle(cr, 0, 0, pixelW, pixelH);
    }

    cairo_set_line_width(cr, strokeWidth_);
    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    if (filled_)
        cairo_fill(cr);
    else
        cairo_stroke(cr);

    cairo_restore(cr);
}

// ────────────── CIRCLE ──────────────

Circle::Circle(const GridCoord& gridPos, float gridRx, float gridRy,
               float duration, float r, float g, float b, float a, float rotation)
    : duration_(duration) {
    gridPos_ = gridPos;
    gridSize_ = {gridRx, gridRy};
    r_ = r; g_ = g; b_ = b; a_ = a;
    rotation_ = rotation;
}

void Circle::draw(cairo_t *cr) {
    if (!g_ctx) return;
    
    extern Scene *g_currentScene;
    if (!g_currentScene) return;
    
    // Convert grid coordinates to pixel coordinates
    float windowWidth = static_cast<float>(g_ctx->width());
    float windowHeight = static_cast<float>(g_ctx->height());
    
    const GridConfig& gridConfig = g_currentScene->getGridConfig();
    float cellWidth = windowWidth / static_cast<float>(gridConfig.cols);
    float cellHeight = windowHeight / static_cast<float>(gridConfig.rows);
    
    float pixelX = (gridPos_.x + 0.5f) * cellWidth;  // Center of grid cell
    float pixelY = (gridPos_.y + 0.5f) * cellHeight;
    float pixelRx = gridSize_.x * cellWidth * 0.5f;
    float pixelRy = gridSize_.y * cellHeight * 0.5f;
    
    cairo_save(cr);
    cairo_translate(cr, pixelX, pixelY);
    cairo_rotate(cr, rotation_);

    cairo_save(cr);
    cairo_scale(cr, pixelRx, pixelRy);
    cairo_arc(cr, 0, 0, 1.0, 0, 2 * M_PI);
    cairo_restore(cr);

    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    if (filled_) {
        cairo_fill(cr);
    } else {
        cairo_set_line_width(cr, strokeWidth_);
        cairo_stroke(cr);
    }

    cairo_restore(cr);
}

// ────────────── LINE ──────────────

Line::Line(const GridCoord& startPos, const GridCoord& endPos,
           float duration, float r, float g, float b, float a)
    : endPos_(endPos), originalEndPos_(endPos), duration_(duration) {
    gridPos_ = startPos;
    gridSize_ = {1.0f, 1.0f}; // Lines don't really have a traditional size
    r_ = r; g_ = g; b_ = b; a_ = a;
    filled_ = false; // Lines are always stroked, not filled
}

void Line::addWaypoint(const GridCoord& waypoint) {
    waypoints_.push_back(waypoint);
}

void Line::setWaypoint(int index, const GridCoord& waypoint) {
    if (index >= 0 && index < static_cast<int>(waypoints_.size())) {
        waypoints_[index] = waypoint;
    }
}

GridCoord Line::getWaypoint(int index) const {
    if (index >= 0 && index < static_cast<int>(waypoints_.size())) {
        return waypoints_[index];
    }
    return {0, 0};
}

void Line::removeWaypoint(int index) {
    if (index >= 0 && index < static_cast<int>(waypoints_.size())) {
        waypoints_.erase(waypoints_.begin() + index);
    }
}

void Line::clearWaypoints() {
    waypoints_.clear();
}

void Line::moveBy(float deltaX, float deltaY) {
    gridPos_.x += deltaX;
    gridPos_.y += deltaY;
    endPos_.x += deltaX;
    endPos_.y += deltaY;
    
    for (auto& waypoint : waypoints_) {
        waypoint.x += deltaX;
        waypoint.y += deltaY;
    }
}

void Line::moveTo(const GridCoord& newStartPos) {
    float deltaX = newStartPos.x - gridPos_.x;
    float deltaY = newStartPos.y - gridPos_.y;
    moveBy(deltaX, deltaY);
}

void Line::draw(cairo_t *cr) {
    if (!g_ctx) return;
    
    extern Scene *g_currentScene;
    if (!g_currentScene) return;
    
    // Convert grid coordinates to pixel coordinates
    float windowWidth = static_cast<float>(g_ctx->width());
    float windowHeight = static_cast<float>(g_ctx->height());
    
    const GridConfig& gridConfig = g_currentScene->getGridConfig();
    float cellWidth = windowWidth / static_cast<float>(gridConfig.cols);
    float cellHeight = windowHeight / static_cast<float>(gridConfig.rows);
    
    cairo_save(cr);
    
    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    cairo_set_line_width(cr, strokeWidth_);
    
    // Start from the start position
    float currentX = (gridPos_.x + 0.5f) * cellWidth;
    float currentY = (gridPos_.y + 0.5f) * cellHeight;
    cairo_move_to(cr, currentX, currentY);
    
    // Draw through all waypoints
    for (const auto& waypoint : waypoints_) {
        float waypointX = (waypoint.x + 0.5f) * cellWidth;
        float waypointY = (waypoint.y + 0.5f) * cellHeight;
        cairo_line_to(cr, waypointX, waypointY);
    }
    
    // End at the end position
    float endPixelX = (endPos_.x + 0.5f) * cellWidth;
    float endPixelY = (endPos_.y + 0.5f) * cellHeight;
    cairo_line_to(cr, endPixelX, endPixelY);
    
    cairo_stroke(cr);
    
    cairo_restore(cr);
}

// ────────────── TEXT ──────────────

Text::Text(const GridCoord& gridPos,
           const std::string& content,
           float fontSize,
           float duration)
    : content_(content), fontSize_(fontSize), duration_(duration), originalFontSize_(fontSize)
{
    gridPos_ = gridPos;
    gridSize_ = {1.0f, 1.0f}; // Text doesn't really have a grid size
}

void Text::setText(const std::string& content) { content_ = content; }
void Text::setFontSize(float size) { fontSize_ = size; }

void Text::draw(cairo_t* cr) {
    if (!g_ctx) return;
    
    extern Scene *g_currentScene;
    if (!g_currentScene) return;
    
    // Convert grid coordinates to pixel coordinates
    float windowWidth = static_cast<float>(g_ctx->width());
    float windowHeight = static_cast<float>(g_ctx->height());
    
    const GridConfig& gridConfig = g_currentScene->getGridConfig();
    float cellWidth = windowWidth / static_cast<float>(gridConfig.cols);
    float cellHeight = windowHeight / static_cast<float>(gridConfig.rows);
    
    float pixelX = (gridPos_.x + 0.5f) * cellWidth;  // Center of grid cell
    float pixelY = (gridPos_.y + 0.5f) * cellHeight;
    
    cairo_save(cr);

    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    cairo_select_font_face(cr, "Sans",
                           CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, fontSize_);

    cairo_move_to(cr, pixelX, pixelY);
    cairo_show_text(cr, content_.c_str());

    cairo_restore(cr);
}

} // namespace banim
