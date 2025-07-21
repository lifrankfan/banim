#include "banim/arch_components.h"
#include <cmath>
#include <algorithm>

namespace banim {

// LogicGate Implementation
LogicGate::LogicGate(float x, float y, Type type, float size) 
    : type_(type), size_(size) {
    x_ = x;
    y_ = y;
}

void LogicGate::draw(cairo_t* cr) {
    cairo_save(cr);
    cairo_translate(cr, x_, y_);
    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    cairo_set_line_width(cr, strokeWidth_);
    
    switch (type_) {
        case AND: drawAndGate(cr); break;
        case OR: drawOrGate(cr); break;
        case NOT: drawNotGate(cr); break;
        // Add other gates as needed
    }
    
    cairo_restore(cr);
}

void LogicGate::drawAndGate(cairo_t* cr) {
    // Draw AND gate shape
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, size_ * 0.6f, 0);
    cairo_arc(cr, size_ * 0.6f, size_ * 0.5f, size_ * 0.5f, -M_PI_2, M_PI_2);
    cairo_line_to(cr, 0, size_);
    cairo_close_path(cr);
    
    if (filled_) cairo_fill(cr);
    else cairo_stroke(cr);
}

void LogicGate::drawOrGate(cairo_t* cr) {
    // Draw OR gate shape - simplified
    cairo_move_to(cr, 0, 0);
    cairo_curve_to(cr, size_ * 0.3f, 0, size_ * 0.7f, 0, size_, size_ * 0.5f);
    cairo_curve_to(cr, size_ * 0.7f, size_, size_ * 0.3f, size_, 0, size_);
    cairo_curve_to(cr, size_ * 0.2f, size_ * 0.7f, size_ * 0.2f, size_ * 0.3f, 0, 0);
    
    if (filled_) cairo_fill(cr);
    else cairo_stroke(cr);
}

void LogicGate::drawNotGate(cairo_t* cr) {
    // Draw NOT gate (triangle with circle)
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, size_ * 0.8f, size_ * 0.5f);
    cairo_line_to(cr, 0, size_);
    cairo_close_path(cr);
    if (filled_) cairo_fill(cr);
    else cairo_stroke(cr);
    
    // Add inverter circle
    cairo_arc(cr, size_ * 0.9f, size_ * 0.5f, size_ * 0.1f, 0, 2 * M_PI);
    cairo_stroke(cr);
}

// Wire Implementation
Wire::Wire(float x1, float y1, float x2, float y2) 
    : x1_(x1), y1_(y1), x2_(x2), y2_(y2) {
    x_ = x1;
    y_ = y1;
}

void Wire::draw(cairo_t* cr) {
    cairo_save(cr);
    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    cairo_set_line_width(cr, strokeWidth_);
    
    // Calculate end point based on draw progress
    float endX = x1_ + (x2_ - x1_) * drawProgress_;
    float endY = y1_ + (y2_ - y1_) * drawProgress_;
    
    cairo_move_to(cr, x1_, y1_);
    cairo_line_to(cr, endX, endY);
    cairo_stroke(cr);
    
    cairo_restore(cr);
}

void Wire::setDrawProgress(float progress) {
    drawProgress_ = std::clamp(progress, 0.0f, 1.0f);
}

// Factory implementations
std::shared_ptr<LogicGate> ArchComponentFactory::createGate(LogicGate::Type type, float x, float y, float size) {
    auto gate = std::make_shared<LogicGate>(x, y, type, size);
    gate->setColor(0.2f, 0.6f, 0.8f); // Default blue
    return gate;
}

std::shared_ptr<Wire> ArchComponentFactory::createWire(float x1, float y1, float x2, float y2) {
    auto wire = std::make_shared<Wire>(x1, y1, x2, y2);
    wire->setColor(0.1f, 0.1f, 0.1f); // Dark wire
    wire->setStrokeWidth(3.0f);
    return wire;
}

// Stub implementations for complex shapes
void LogicGate::setSize(float w, float h) { size_ = w; }
void LogicGate::getAnimatableSize(float& w, float& h) const { w = h = size_; }
void LogicGate::setAnimatableSize(float w, float h) { size_ = w; }
void LogicGate::resetForAnimation() {}

void Wire::setSize(float w, float h) {}
void Wire::getAnimatableSize(float& w, float& h) const { w = abs(x2_ - x1_); h = abs(y2_ - y1_); }
void Wire::setAnimatableSize(float w, float h) {}
void Wire::resetForAnimation() {}

} // namespace banim
