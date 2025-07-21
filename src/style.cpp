#include "banim/style.h"

namespace banim {

StyleManager& StyleManager::instance() {
    static StyleManager instance;
    return instance;
}

void StyleManager::loadArchitectureTheme() {
    // CPU component colors
    setColor("cpu_alu", Color(0.2f, 0.6f, 0.8f, 1.0f));      // Blue
    setColor("cpu_control", Color(0.8f, 0.4f, 0.2f, 1.0f));  // Orange
    setColor("cpu_register", Color(0.6f, 0.8f, 0.3f, 1.0f)); // Green
    
    // Memory colors
    setColor("memory_cache", Color(0.9f, 0.7f, 0.2f, 1.0f)); // Yellow
    setColor("memory_ram", Color(0.7f, 0.3f, 0.8f, 1.0f));   // Purple
    setColor("memory_storage", Color(0.5f, 0.5f, 0.5f, 1.0f)); // Gray
    
    // Data path colors
    setColor("datapath_address", Color(1.0f, 0.2f, 0.2f, 1.0f)); // Red
    setColor("datapath_data", Color(0.2f, 0.8f, 0.2f, 1.0f));    // Green
    setColor("datapath_control", Color(0.2f, 0.2f, 1.0f, 1.0f)); // Blue
    
    // Common styles
    setStyle("default_animation_duration", 0.8f);
    setStyle("wire_thickness", 3.0f);
    setStyle("component_border_radius", 8.0f);
}

void StyleManager::setStyle(const std::string& key, const StyleValue& value) {
    styles_[key] = value;
}

StyleManager::StyleValue StyleManager::getStyle(const std::string& key) const {
    auto it = styles_.find(key);
    return it != styles_.end() ? it->second : StyleValue{};
}

float StyleManager::getFloat(const std::string& key, float defaultVal) const {
    auto style = getStyle(key);
    if (std::holds_alternative<float>(style)) {
        return std::get<float>(style);
    }
    return defaultVal;
}

void StyleManager::setColor(const std::string& name, const Color& color) {
    colors_[name] = color;
}

StyleManager::Color StyleManager::getColor(const std::string& name) const {
    auto it = colors_.find(name);
    return it != colors_.end() ? it->second : Color{};
}

// Predefined colors
namespace Colors {
    const StyleManager::Color CPU_ALU(0.2f, 0.6f, 0.8f, 1.0f);
    const StyleManager::Color CPU_CONTROL(0.8f, 0.4f, 0.2f, 1.0f);
    const StyleManager::Color CPU_REGISTER(0.6f, 0.8f, 0.3f, 1.0f);
    
    const StyleManager::Color MEMORY_CACHE(0.9f, 0.7f, 0.2f, 1.0f);
    const StyleManager::Color MEMORY_RAM(0.7f, 0.3f, 0.8f, 1.0f);
    const StyleManager::Color MEMORY_STORAGE(0.5f, 0.5f, 0.5f, 1.0f);
    
    const StyleManager::Color DATAPATH_ADDRESS(1.0f, 0.2f, 0.2f, 1.0f);
    const StyleManager::Color DATAPATH_DATA(0.2f, 0.8f, 0.2f, 1.0f);
    const StyleManager::Color DATAPATH_CONTROL(0.2f, 0.2f, 1.0f, 1.0f);
}

} // namespace banim
