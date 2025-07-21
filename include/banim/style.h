#pragma once

#include <unordered_map>
#include <string>
#include <variant>

namespace banim {

// Style system for consistent theming across computer architecture diagrams
class StyleManager {
public:
    using StyleValue = std::variant<float, int, std::string>;
    
    static StyleManager& instance();
    
    // Predefined styles for computer architecture
    void loadArchitectureTheme();
    void loadDarkTheme();
    void loadLightTheme();
    
    // Style setters/getters
    void setStyle(const std::string& key, const StyleValue& value);
    StyleValue getStyle(const std::string& key) const;
    
    // Convenience methods for common styles
    float getFloat(const std::string& key, float defaultVal = 0.0f) const;
    int getInt(const std::string& key, int defaultVal = 0) const;
    std::string getString(const std::string& key, const std::string& defaultVal = "") const;
    
    // Color management
    struct Color {
        float r, g, b, a;
        Color(float r = 0, float g = 0, float b = 0, float a = 1) : r(r), g(g), b(b), a(a) {}
    };
    
    void setColor(const std::string& name, const Color& color);
    Color getColor(const std::string& name) const;
    
private:
    std::unordered_map<std::string, StyleValue> styles_;
    std::unordered_map<std::string, Color> colors_;
    
    StyleManager() = default;
};

// Predefined color schemes for computer architecture
namespace Colors {
    // CPU Components
    extern const StyleManager::Color CPU_ALU;
    extern const StyleManager::Color CPU_CONTROL;
    extern const StyleManager::Color CPU_REGISTER;
    
    // Memory
    extern const StyleManager::Color MEMORY_CACHE;
    extern const StyleManager::Color MEMORY_RAM;
    extern const StyleManager::Color MEMORY_STORAGE;
    
    // Data paths
    extern const StyleManager::Color DATAPATH_ADDRESS;
    extern const StyleManager::Color DATAPATH_DATA;
    extern const StyleManager::Color DATAPATH_CONTROL;
    
    // Logic gates
    extern const StyleManager::Color GATE_INPUT;
    extern const StyleManager::Color GATE_OUTPUT;
    extern const StyleManager::Color GATE_LOGIC;
}

} // namespace banim
