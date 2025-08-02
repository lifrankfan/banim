#pragma once
#include "banim/shapes.h"
#include "banim/grid.h"
#include <string>
#include <cairo/cairo.h>
#include "animations.h"

namespace banim {

class Text : public Shape {
public:
    // Pixel-based constructor
    Text(float x, float y,
         const std::string& content,
         float fontSize = 24.0f,
         float duration = default_duration);
         
    // Grid-based constructor
    Text(const GridCoord& gridPos,
         const std::string& content,
         float fontSize = 24.0f,
         const Scene* scene = nullptr,
         float duration = default_duration);

    void setText(const std::string& content);
    void setFontSize(float size);
    float getFontSize() const { return fontSize_; }
    void draw(cairo_t* cr) override;
    void setSize(float w, float h) override;

    void getAnimatableSize(float& w, float& h) const override {
        w = fontSize_;
        h = fontSize_;
    }
    
    void setAnimatableSize(float w, float h) override {
        // For text, we scale by changing the font size
        // and reposition to keep the visual center fixed
        
        setFontSize(w);  // Use width as the new font size
        
        // Calculate new text dimensions
        float textWidth = content_.length() * fontSize_ * 0.6f;
        float textHeight = fontSize_;
        
        // Position the text so its visual center stays at centerX_, centerY_
        x_ = centerX_ - textWidth * 0.5f;
        y_ = centerY_ + textHeight * 0.3f;  // Adjust for baseline positioning
    }
    
    void resetForAnimation() override {
        // Calculate current text dimensions and store the visual center
        float textWidth = content_.length() * fontSize_ * 0.6f;
        float textHeight = fontSize_;
        
        // Store the visual center based on current position
        centerX_ = x_ + textWidth * 0.5f;
        centerY_ = y_ - textHeight * 0.3f;  // Move up from baseline to visual center
    }

private:
    std::string content_;
    float fontSize_;
    float duration_;
    float centerX_ = 0, centerY_ = 0;
};

} // namespace banim
