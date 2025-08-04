#include "banim/logic_gates.h"
#include "banim/scene.h"
#include "banim/init.h"
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace banim {

LogicGate::LogicGate(GateType type, PortDirection facing, const GridCoord& position, 
                     float gridWidth, float gridHeight)
    : Rectangle(position, gridWidth, gridHeight, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f), gateType_(type), facing_(facing) {
    
    // Set default gate appearance
    setColor(gateR_, gateG_, gateB_, gateA_);
    
    setupPorts();
}

void LogicGate::setupPorts() {
    // Clear any existing ports
    leftPorts_.clear();
    rightPorts_.clear();
    topPorts_.clear();
    bottomPorts_.clear();
    
    // Setup ports based on gate type and facing direction
    if (gateType_ == GateType::NOT) {
        // NOT gate: 1 input, 1 output
        switch (facing_) {
            case PortDirection::RIGHT:
                addPort(PortDirection::LEFT, "input");
                addPort(PortDirection::RIGHT, "output");
                break;
            case PortDirection::LEFT:
                addPort(PortDirection::RIGHT, "input");
                addPort(PortDirection::LEFT, "output");
                break;
            case PortDirection::TOP:
                addPort(PortDirection::BOTTOM, "input");
                addPort(PortDirection::TOP, "output");
                break;
            case PortDirection::BOTTOM:
                addPort(PortDirection::TOP, "input");
                addPort(PortDirection::BOTTOM, "output");
                break;
        }
    } else {
        // Two-input gates: 2 inputs, 1 output
        switch (facing_) {
            case PortDirection::RIGHT:
                addPort(PortDirection::LEFT, "input1");
                addPort(PortDirection::LEFT, "input2");
                addPort(PortDirection::RIGHT, "output");
                break;
            case PortDirection::LEFT:
                addPort(PortDirection::RIGHT, "input1");
                addPort(PortDirection::RIGHT, "input2");
                addPort(PortDirection::LEFT, "output");
                break;
            case PortDirection::TOP:
                addPort(PortDirection::BOTTOM, "input1");
                addPort(PortDirection::BOTTOM, "input2");
                addPort(PortDirection::TOP, "output");
                break;
            case PortDirection::BOTTOM:
                addPort(PortDirection::TOP, "input1");
                addPort(PortDirection::TOP, "input2");
                addPort(PortDirection::BOTTOM, "output");
                break;
        }
    }
    
    updatePortPositions();
}

void LogicGate::setFacing(PortDirection facing) {
    facing_ = facing;
    setupPorts();
}

// Port management methods
void LogicGate::addPort(PortDirection direction, const std::string& name) {
    getPortVector(direction).emplace_back(direction, name);
    updatePortPositions();
}

void LogicGate::removePort(PortDirection direction, const std::string& name) {
    auto& ports = getPortVector(direction);
    ports.erase(std::remove_if(ports.begin(), ports.end(),
        [&name](const Port& port) { return port.name == name; }), ports.end());
    updatePortPositions();
}

void LogicGate::clearPorts(PortDirection direction) {
    getPortVector(direction).clear();
    updatePortPositions();
}

void LogicGate::clearAllPorts() {
    leftPorts_.clear();
    rightPorts_.clear();
    topPorts_.clear();
    bottomPorts_.clear();
}

std::vector<Port>& LogicGate::getPorts(PortDirection direction) {
    return getPortVector(direction);
}

const std::vector<Port>& LogicGate::getPorts(PortDirection direction) const {
    return getPortVector(direction);
}

Port* LogicGate::getPort(PortDirection direction, const std::string& name) {
    auto& ports = getPortVector(direction);
    for (auto& port : ports) {
        if (port.name == name) {
            return &port;
        }
    }
    return nullptr;
}

Port* LogicGate::getPort(PortDirection direction, int index) {
    auto& ports = getPortVector(direction);
    if (index >= 0 && index < static_cast<int>(ports.size())) {
        return &ports[index];
    }
    return nullptr;
}

std::vector<Port>& LogicGate::getPortVector(PortDirection direction) {
    switch (direction) {
        case PortDirection::LEFT:   return leftPorts_;
        case PortDirection::RIGHT:  return rightPorts_;
        case PortDirection::TOP:    return topPorts_;
        case PortDirection::BOTTOM: return bottomPorts_;
        default:                    return leftPorts_;
    }
}

const std::vector<Port>& LogicGate::getPortVector(PortDirection direction) const {
    switch (direction) {
        case PortDirection::LEFT:   return leftPorts_;
        case PortDirection::RIGHT:  return rightPorts_;
        case PortDirection::TOP:    return topPorts_;
        case PortDirection::BOTTOM: return bottomPorts_;
        default:                    return leftPorts_;
    }
}

void LogicGate::updatePortPositions() {
    // Use gate-specific port positioning logic
    updatePortsForDirection(getPorts(PortDirection::LEFT), PortDirection::LEFT);
    updatePortsForDirection(getPorts(PortDirection::RIGHT), PortDirection::RIGHT);
    updatePortsForDirection(getPorts(PortDirection::TOP), PortDirection::TOP);
    updatePortsForDirection(getPorts(PortDirection::BOTTOM), PortDirection::BOTTOM);
}

void LogicGate::updatePortsForDirection(std::vector<Port>& ports, PortDirection direction) {
    if (ports.empty()) return;
    
    size_t numPorts = ports.size();
    
    for (size_t i = 0; i < numPorts; ++i) {
        float ratio;
        
        if (numPorts == 1) {
            // Single port - center it
            ratio = 0.5f;
        } else {
            // Multiple ports - distribute with proper padding from edges
            float padding = 0.25f;  // 20% padding from each edge
            float usableSpace = 1.0f - (2.0f * padding);
            ratio = padding + (static_cast<float>(i) / (numPorts - 1)) * usableSpace;
        }
        
        // Position ports based on gate geometry - gates fill the entire grid cell
        float actualWidth = getGridWidth();
        float actualHeight = getGridHeight();
        float offsetX = 0.0f;
        float offsetY = 0.0f;
        
        switch (direction) {
            case PortDirection::LEFT:
                // Input side - all gates have inputs at the left edge
                ports[i].position.x = gridPos_.x + offsetX;
                ports[i].position.y = gridPos_.y + offsetY + ratio * actualHeight;
                break;
                
            case PortDirection::RIGHT:
            {
                // Output is on the right side - all gates have outputs at the right edge
                ports[i].position.x = gridPos_.x + offsetX + actualWidth;
                if (ports.size() == 1) {
                    // Single output port - center it
                    ports[i].position.y = gridPos_.y + offsetY + 0.5f * actualHeight;
                } else {
                    // Multiple output ports - distribute with padding
                    ports[i].position.y = gridPos_.y + offsetY + ratio * actualHeight;
                }
                break;
            }
                
            case PortDirection::TOP:
                ports[i].position.x = gridPos_.x + offsetX + ratio * actualWidth;
                ports[i].position.y = gridPos_.y + offsetY;
                break;
                
            case PortDirection::BOTTOM:
                ports[i].position.x = gridPos_.x + offsetX + ratio * actualWidth;
                ports[i].position.y = gridPos_.y + offsetY + actualHeight;
                break;
        }
    }
}

void LogicGate::setGateColor(float r, float g, float b, float a) {
    gateR_ = r; gateG_ = g; gateB_ = b; gateA_ = a;
    setColor(r, g, b, a);
}

void LogicGate::setSymbolColor(float r, float g, float b, float a) {
    symbolR_ = r; symbolG_ = g; symbolB_ = b; symbolA_ = a;
}

void LogicGate::draw(cairo_t* cr) {
    // Get pixel coordinates and size
    extern GLContext* g_ctx;
    if (!g_ctx) return;
    
    extern Scene *g_currentScene;
    if (!g_currentScene) return;
    
    float windowWidth = static_cast<float>(g_ctx->width());
    float windowHeight = static_cast<float>(g_ctx->height());
    
    const GridConfig& gridConfig = g_currentScene->getGridConfig();
    float cellWidth = windowWidth / static_cast<float>(gridConfig.cols);
    float cellHeight = windowHeight / static_cast<float>(gridConfig.rows);
    
    // Use the same coordinate system as Rectangle
    float pixelX = gridPos_.x * cellWidth;
    float pixelY = gridPos_.y * cellHeight;
    float pixelW = gridSize_.x * cellWidth;
    float pixelH = gridSize_.y * cellHeight;
    
    // Set up drawing context - gates fill the entire grid cell
    cairo_save(cr);
    cairo_translate(cr, pixelX, pixelY);
    
    // Use the full grid cell size
    float gateWidth = pixelW;
    float gateHeight = pixelH;
    
    // Apply rotation based on facing direction
    switch (facing_) {
        case PortDirection::RIGHT:
            // Default orientation, no rotation needed
            break;
        case PortDirection::LEFT:
            // Rotate 180 degrees
            cairo_translate(cr, gateWidth / 2, gateHeight / 2);
            cairo_rotate(cr, M_PI);
            cairo_translate(cr, -gateWidth / 2, -gateHeight / 2);
            break;
        case PortDirection::TOP:
            // Rotate 90 degrees counter-clockwise
            cairo_translate(cr, gateWidth / 2, gateHeight / 2);
            cairo_rotate(cr, -M_PI_2);
            cairo_translate(cr, -gateHeight / 2, -gateWidth / 2);
            // Swap width and height for vertical orientation
            std::swap(gateWidth, gateHeight);
            break;
        case PortDirection::BOTTOM:
            // Rotate 90 degrees clockwise
            cairo_translate(cr, gateWidth / 2, gateHeight / 2);
            cairo_rotate(cr, M_PI_2);
            cairo_translate(cr, -gateHeight / 2, -gateWidth / 2);
            // Swap width and height for vertical orientation
            std::swap(gateWidth, gateHeight);
            break;
    }
    
    // Set colors and style - use inherited alpha for animations like Rectangle does
    cairo_set_source_rgba(cr, gateR_, gateG_, gateB_, gateA_ * a_);
    cairo_set_line_width(cr, 2.0f);
    
    // Draw the appropriate gate shape
    switch (gateType_) {
        case GateType::AND:
            drawAndGate(cr, gateWidth, gateHeight);
            break;
        case GateType::OR:
            drawOrGate(cr, gateWidth, gateHeight);
            break;
        case GateType::XOR:
            drawXorGate(cr, gateWidth, gateHeight);
            break;
        case GateType::NOT:
            drawNotGate(cr, gateWidth, gateHeight);
            break;
        case GateType::NAND:
            drawAndGate(cr, gateWidth, gateHeight);
            drawInversionBubble(cr, gateWidth, gateHeight);
            break;
        case GateType::NOR:
            drawOrGate(cr, gateWidth, gateHeight);
            drawInversionBubble(cr, gateWidth, gateHeight);
            break;
        case GateType::XNOR:
            drawXorGate(cr, gateWidth, gateHeight);
            drawInversionBubble(cr, gateWidth, gateHeight);
            break;
    }
    
    cairo_restore(cr);
}

void LogicGate::drawAndGate(cairo_t* cr, float width, float height) {
    // Draw AND gate shape - rectangle on left, semicircle on right
    // Make sure the semicircle doesn't extend beyond the gate width
    float radius = height * 0.5f;
    float rectWidth = width - radius;  // Rectangle width = total width minus semicircle radius
    
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, rectWidth, 0);
    cairo_arc(cr, rectWidth, height * 0.5f, radius, -M_PI_2, M_PI_2);
    cairo_line_to(cr, 0, height);
    cairo_line_to(cr, 0, 0);
    
    if (filled_) cairo_fill(cr);
    else cairo_stroke(cr);
}

void LogicGate::drawOrGate(cairo_t* cr, float width, float height) {
    // Draw OR gate shape - extend only to the left, stay within grid on the right
    float gateWidth = width * 1.1f;  // Make gate 10% wider
    float offsetX = -width * 0.1f;   // Shift left by 10% so it extends only leftward
    
    cairo_save(cr);
    cairo_translate(cr, offsetX, 0);
    
    cairo_move_to(cr, 0, 0);
    cairo_curve_to(cr, gateWidth * 0.3f, 0, gateWidth * 0.7f, 0, gateWidth, height * 0.5f);
    cairo_curve_to(cr, gateWidth * 0.7f, height, gateWidth * 0.3f, height, 0, height);
    cairo_curve_to(cr, gateWidth * 0.2f, height * 0.7f, gateWidth * 0.2f, height * 0.3f, 0, 0);
    
    if (filled_) cairo_fill(cr);
    else cairo_stroke(cr);
    
    cairo_restore(cr);
}

void LogicGate::drawXorGate(cairo_t* cr, float width, float height) {
    // Draw XOR gate shape - extend only to the left, stay within grid on the right
    float gateWidth = width * 1.05f;  // Make gate 5% wider
    float offsetX = -width * 0.05f;   // Shift left by 5% so it extends only leftward
    
    cairo_save(cr);
    cairo_translate(cr, offsetX, 0);
    
    // Draw the main OR gate shape (shifted right slightly)
    cairo_save(cr);
    cairo_translate(cr, gateWidth * 0.1f, 0);
    
    // Draw main OR shape
    float mainGateWidth = gateWidth * 0.9f;
    cairo_move_to(cr, 0, 0);
    cairo_curve_to(cr, mainGateWidth * 0.3f, 0, mainGateWidth * 0.7f, 0, mainGateWidth, height * 0.5f);
    cairo_curve_to(cr, mainGateWidth * 0.7f, height, mainGateWidth * 0.3f, height, 0, height);
    cairo_curve_to(cr, mainGateWidth * 0.2f, height * 0.7f, mainGateWidth * 0.2f, height * 0.3f, 0, 0);
    
    if (filled_) cairo_fill(cr);
    else cairo_stroke(cr);
    cairo_restore(cr);
    
    // Draw the additional curved line for XOR
    cairo_move_to(cr, 0, height * 0.2f);
    cairo_curve_to(cr,
        gateWidth * 0.15f, height * 0.35f,
        gateWidth * 0.15f, height * 0.65f,
        0, height * 0.8f);
    cairo_stroke(cr);
    
    cairo_restore(cr);
}

void LogicGate::drawNotGate(cairo_t* cr, float width, float height) {
    // Draw NOT gate (triangle)
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, width * 0.8f, height * 0.5f);
    cairo_line_to(cr, 0, height);
    cairo_line_to(cr, 0, 0);
    if (filled_) cairo_fill(cr);
    else cairo_stroke(cr);
    
    // Add inverter circle
    cairo_arc(cr, width * 0.9f, height * 0.5f, height * 0.1f, 0, 2 * M_PI);
    if (filled_) {
        cairo_set_source_rgba(cr, 1.0f, 1.0f, 1.0f, 1.0f); // White fill for bubble
        cairo_fill_preserve(cr);
        cairo_set_source_rgba(cr, gateR_, gateG_, gateB_, gateA_ * a_); // Restore gate color
    }
    cairo_stroke(cr);
}

void LogicGate::drawInversionBubble(cairo_t* cr, float width, float height) {
    // Draw inversion bubble at the output - simple positioning at the right edge
    float bubbleRadius = height * 0.08f;
    float bubbleX = width;  // Near the right edge
    float bubbleY = height * 0.5f;  // Centered vertically
    
    cairo_arc(cr, bubbleX, bubbleY, bubbleRadius, 0, 2 * M_PI);
    
    if (filled_) {
        cairo_set_source_rgba(cr, 1.0f, 1.0f, 1.0f, 1.0f); // White fill for bubble
        cairo_fill_preserve(cr);
        cairo_set_source_rgba(cr, gateR_, gateG_, gateB_, gateA_ * a_); // Restore gate color
    }
    cairo_stroke(cr);
}

std::string LogicGate::getGateSymbol() const {
    switch (gateType_) {
        case GateType::AND:  return "&";
        case GateType::OR:   return "≥1";
        case GateType::XOR:  return "=1";
        case GateType::NOT:  return "1";
        case GateType::NAND: return "&̄";   // AND with overline
        case GateType::NOR:  return "≥1̄";  // OR with overline
        case GateType::XNOR: return "=1̄";  // XOR with overline
        default: return "?";
    }
}

} // namespace banim
