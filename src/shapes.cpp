#include "banim/shapes.h"
#include "banim/animations.h"
#include "banim/scene.h"
#include <cmath>
#include <memory>

namespace banim {

// ────────────── SHAPE BASE CLASS ──────────────

void Shape::setGridPos(const GridCoord& coord, const Scene* scene) {
    setGridPos(coord.x, coord.y, scene);
}

void Shape::setGridPos(float gridX, float gridY, const Scene* scene) {
    if (!scene) return;
    auto [pixelX, pixelY] = scene->gridToPixel(gridX, gridY);
    setPos(pixelX, pixelY);
}

GridCoord Shape::getGridPos(const Scene* scene) const {
    if (!scene) return {0, 0};
    return scene->pixelToGrid(x_, y_);
}

// ────────────── RECTANGLE ──────────────

Rectangle::Rectangle(float x, float y, float width, float height,
                     float duration, float r, float g, float b, float a,
                     float rotation) : duration_(duration) {
    x_ = x; y_ = y;
    w_ = width; h_ = height;
    r_ = r; g_ = g; b_ = b; a_ = a;
    rotation_ = rotation;
}

Rectangle::Rectangle(const GridCoord& gridPos, float gridWidth, float gridHeight,
                     float duration, float r, float g, float b, float a, float rotation) 
    : duration_(duration) {
    r_ = r; g_ = g; b_ = b; a_ = a;
    rotation_ = rotation;
    coordinateSystem_ = CoordinateSystem::GRID;
    gridPos_ = gridPos;
    gridSize_ = {gridWidth, gridHeight};
    
    // Set temporary values - will be updated when added to scene
    x_ = 0; y_ = 0;
    w_ = 100; h_ = 100; // Default size
}

Rectangle& Rectangle::setBorderRadius(float radius) {
    borderRadius_ = radius;
    return *this;
}

void Rectangle::setPos(float x, float y) {
    x_ = x;
    y_ = y;
}

void Rectangle::setSize(float w, float h) {
    w_ = w;
    h_ = h;
}

void Rectangle::setGridSize(float gridWidth, float gridHeight, const Scene* scene) {
    if (!scene) {
        setSize(gridWidth, gridHeight);
        return;
    }
    
    auto [cellWidth, cellHeight] = scene->getGridCellSize();
    setSize(gridWidth * cellWidth, gridHeight * cellHeight);
}

void Rectangle::draw(cairo_t* cr) {
    cairo_save(cr);
    cairo_translate(cr, x_ + w_ / 2, y_ + h_ / 2);
    cairo_rotate(cr, rotation_);
    cairo_translate(cr, -w_ / 2, -h_ / 2);

    if (borderRadius_ > 0.0f) {
        float r = std::min(borderRadius_, std::min(w_ / 2, h_ / 2));
        cairo_new_sub_path(cr);
        cairo_arc(cr, w_ - r, r, r, -M_PI_2, 0);
        cairo_arc(cr, w_ - r, h_ - r, r, 0, M_PI_2);
        cairo_arc(cr, r, h_ - r, r, M_PI_2, M_PI);
        cairo_arc(cr, r, r, r, M_PI, 3 * M_PI_2);
        cairo_close_path(cr);
    } else {
        cairo_rectangle(cr, 0, 0, w_, h_);
    }

    cairo_set_line_width(cr, strokeWidth_);
    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    if (filled_)
        cairo_fill(cr);
    else
        cairo_stroke(cr);

    cairo_restore(cr);
}

void Rectangle::updateFromGrid(const Scene* scene) {
    if (coordinateSystem_ == CoordinateSystem::GRID && scene) {
        auto [cellWidth, cellHeight] = scene->getGridCellSize();
        w_ = gridSize_.first * cellWidth;
        h_ = gridSize_.second * cellHeight;
        
        auto [pixelX, pixelY] = scene->gridToPixel(gridPos_);
        x_ = pixelX - w_ / 2.0f;  // Center the rectangle on the grid position
        y_ = pixelY - h_ / 2.0f;
    }
}

// ────────────── CIRCLE ──────────────

Circle::Circle(float cx, float cy, float rx, float ry,
               float duration, float r, float g, float b, float a,
               float rotation) : duration_(duration) {
    x_ = cx; y_ = cy;
    rx_ = rx; ry_ = ry;
    r_ = r; g_ = g; b_ = b; a_ = a;
    rotation_ = rotation;
}

Circle::Circle(const GridCoord& gridPos, float gridRx, float gridRy,
               float duration, float r, float g, float b, float a, float rotation)
    : duration_(duration) {
    r_ = r; g_ = g; b_ = b; a_ = a;
    rotation_ = rotation;
    coordinateSystem_ = CoordinateSystem::GRID;
    gridPos_ = gridPos;
    gridSize_ = {gridRx, gridRy};
    
    // Set temporary values - will be updated when added to scene
    x_ = 0; y_ = 0;
    rx_ = 50; ry_ = 50; // Default radius
}

void Circle::setPos(float x, float y) {
    x_ = x;
    y_ = y;
}

void Circle::setSize(float rx, float ry) {
    rx_ = rx;
    ry_ = ry;
}

void Circle::setGridSize(float gridRx, float gridRy, const Scene* scene) {
    if (!scene) {
        setSize(gridRx, gridRy);
        return;
    }
    
    auto [cellWidth, cellHeight] = scene->getGridCellSize();
    setSize(gridRx * cellWidth * 0.5f, gridRy * cellHeight * 0.5f);
}

void Circle::draw(cairo_t *cr) {
    cairo_save(cr);
    cairo_translate(cr, x_, y_);
    cairo_rotate(cr, rotation_);

    cairo_save(cr);
    cairo_scale(cr, rx_, ry_);
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

void Circle::updateFromGrid(const Scene* scene) {
    if (coordinateSystem_ == CoordinateSystem::GRID && scene) {
        auto [cellWidth, cellHeight] = scene->getGridCellSize();
        rx_ = gridSize_.first * cellWidth;
        ry_ = gridSize_.second * cellHeight;
        
        auto [pixelX, pixelY] = scene->gridToPixel(gridPos_);
        x_ = pixelX;  // Circle center is at the grid position
        y_ = pixelY;
    }
}

} // namespace banim
