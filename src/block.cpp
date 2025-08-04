#include "banim/block.h"
#include "banim/scene.h"
#include "banim/init.h"
#include <cairo.h>
#include <algorithm>
#include <cmath>

namespace banim {

Block::Block(const GridCoord& position, float gridWidth, float gridHeight, 
             const std::string& label)
    : Rectangle(position, gridWidth, gridHeight), 
      label_(label), labelSize_(16.0f), 
      labelR_(1.0f), labelG_(1.0f), labelB_(1.0f), labelA_(1.0f) {
    
    // Set default appearance for blocks
    setColor(0.3f, 0.3f, 0.7f, 1.0f);
    setFilled(true);
    setBorderRadius(8.0f);
    setStrokeWidth(2.0f);
}

void Block::draw(cairo_t* cr) {
    // Draw the block background (use parent Rectangle drawing)
    Rectangle::draw(cr);
    
    // Get grid-to-pixel conversion using current scene's grid config
    if (!g_ctx) return;
    
    extern Scene *g_currentScene;
    if (!g_currentScene) return;
    
    float windowWidth = static_cast<float>(g_ctx->width());
    float windowHeight = static_cast<float>(g_ctx->height());
    
    const GridConfig& gridConfig = g_currentScene->getGridConfig();
    float cellWidth = windowWidth / static_cast<float>(gridConfig.cols);
    float cellHeight = windowHeight / static_cast<float>(gridConfig.rows);
    
    float pixelX = gridPos_.x * cellWidth;
    float pixelY = gridPos_.y * cellHeight;
    float pixelW = gridSize_.x * cellWidth;
    float pixelH = gridSize_.y * cellHeight;
    
    // Draw the label
    if (!label_.empty()) {
        cairo_save(cr);
        
        // Set text properties
        cairo_set_source_rgba(cr, labelR_, labelG_, labelB_, labelA_);
        cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, labelSize_);
        
        // Get text dimensions for centering
        cairo_text_extents_t textExtents;
        cairo_text_extents(cr, label_.c_str(), &textExtents);
        
        // Center the text in the block
        float textX = pixelX + (pixelW - textExtents.width) / 2.0f - textExtents.x_bearing;
        float textY = pixelY + (pixelH + textExtents.height) / 2.0f - textExtents.y_bearing;
        
        cairo_move_to(cr, textX, textY);
        cairo_show_text(cr, label_.c_str());
        
        cairo_restore(cr);
    }
    
    // Draw port indicators (small circles)
    cairo_save(cr);
    cairo_set_source_rgba(cr, 0.8f, 0.8f, 0.8f, 1.0f);
    
    // Draw all ports
    for (const auto& port : leftPorts_) {
        float portPixelX = port.position.x * cellWidth;
        float portPixelY = port.position.y * cellHeight;
        cairo_arc(cr, portPixelX, portPixelY, 3.0f, 0, 2 * M_PI);
        cairo_fill(cr);
    }
    for (const auto& port : rightPorts_) {
        float portPixelX = port.position.x * cellWidth;
        float portPixelY = port.position.y * cellHeight;
        cairo_arc(cr, portPixelX, portPixelY, 3.0f, 0, 2 * M_PI);
        cairo_fill(cr);
    }
    for (const auto& port : topPorts_) {
        float portPixelX = port.position.x * cellWidth;
        float portPixelY = port.position.y * cellHeight;
        cairo_arc(cr, portPixelX, portPixelY, 3.0f, 0, 2 * M_PI);
        cairo_fill(cr);
    }
    for (const auto& port : bottomPorts_) {
        float portPixelX = port.position.x * cellWidth;
        float portPixelY = port.position.y * cellHeight;
        cairo_arc(cr, portPixelX, portPixelY, 3.0f, 0, 2 * M_PI);
        cairo_fill(cr);
    }
    
    cairo_restore(cr);
}

void Block::addPort(PortDirection direction, const std::string& name) {
    std::vector<Port>& ports = getPortVector(direction);
    ports.emplace_back(direction, name);
    updatePortPositions();
}

void Block::removePort(PortDirection direction, const std::string& name) {
    std::vector<Port>& ports = getPortVector(direction);
    auto it = std::find_if(ports.begin(), ports.end(),
        [&name](const Port& port) { return port.name == name; });
    
    if (it != ports.end()) {
        ports.erase(it);
        updatePortPositions();
    }
}

void Block::clearPorts(PortDirection direction) {
    getPortVector(direction).clear();
    updatePortPositions();
}

void Block::clearAllPorts() {
    leftPorts_.clear();
    rightPorts_.clear();
    topPorts_.clear();
    bottomPorts_.clear();
    updatePortPositions();
}

std::vector<Port>& Block::getPorts(PortDirection direction) {
    return getPortVector(direction);
}

const std::vector<Port>& Block::getPorts(PortDirection direction) const {
    return getPortVector(direction);
}

Port* Block::getPort(PortDirection direction, const std::string& name) {
    std::vector<Port>& ports = getPortVector(direction);
    auto it = std::find_if(ports.begin(), ports.end(),
        [&name](const Port& port) { return port.name == name; });
    
    return (it != ports.end()) ? &(*it) : nullptr;
}

Port* Block::getPort(PortDirection direction, int index) {
    std::vector<Port>& ports = getPortVector(direction);
    return (index >= 0 && index < static_cast<int>(ports.size())) ? &ports[index] : nullptr;
}

void Block::setLabel(const std::string& label) {
    label_ = label;
}

void Block::setLabelColor(float r, float g, float b, float a) {
    labelR_ = r;
    labelG_ = g;
    labelB_ = b;
    labelA_ = a;
}

void Block::updatePortPositions() {
    updatePortsForDirection(leftPorts_, PortDirection::LEFT);
    updatePortsForDirection(rightPorts_, PortDirection::RIGHT);
    updatePortsForDirection(topPorts_, PortDirection::TOP);
    updatePortsForDirection(bottomPorts_, PortDirection::BOTTOM);
}

void Block::updatePortsForDirection(std::vector<Port>& ports, PortDirection direction) {
    if (ports.empty()) return;
    
    size_t numPorts = ports.size();
    
    for (size_t i = 0; i < numPorts; ++i) {
        float ratio;
        
        if (numPorts == 1) {
            // Single port - center it
            ratio = 0.5f;
        } else {
            // Multiple ports - distribute with padding from edges
            float padding = 0.15f;  // 15% padding from each edge
            float usableSpace = 1.0f - (2.0f * padding);
            ratio = padding + (static_cast<float>(i) / (numPorts - 1)) * usableSpace;
        }
        
        switch (direction) {
            case PortDirection::LEFT:
                ports[i].position.x = gridPos_.x;
                ports[i].position.y = gridPos_.y + ratio * gridSize_.y;
                break;
                
            case PortDirection::RIGHT:
                ports[i].position.x = gridPos_.x + gridSize_.x;
                ports[i].position.y = gridPos_.y + ratio * gridSize_.y;
                break;
                
            case PortDirection::TOP:
                ports[i].position.x = gridPos_.x + ratio * gridSize_.x;
                ports[i].position.y = gridPos_.y;
                break;
                
            case PortDirection::BOTTOM:
                ports[i].position.x = gridPos_.x + ratio * gridSize_.x;
                ports[i].position.y = gridPos_.y + gridSize_.y;
                break;
        }
    }
}

std::vector<Port>& Block::getPortVector(PortDirection direction) {
    switch (direction) {
        case PortDirection::LEFT:   return leftPorts_;
        case PortDirection::RIGHT:  return rightPorts_;
        case PortDirection::TOP:    return topPorts_;
        case PortDirection::BOTTOM: return bottomPorts_;
        default:                    return leftPorts_;
    }
}

const std::vector<Port>& Block::getPortVector(PortDirection direction) const {
    switch (direction) {
        case PortDirection::LEFT:   return leftPorts_;
        case PortDirection::RIGHT:  return rightPorts_;
        case PortDirection::TOP:    return topPorts_;
        case PortDirection::BOTTOM: return bottomPorts_;
        default:                    return leftPorts_;
    }
}

} // namespace banim
