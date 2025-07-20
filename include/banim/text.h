#pragma once
#include "banim/shapes.h"
#include <string>
#include <cairo/cairo.h>
#include "animations.h"

namespace banim {

class Text : public Shape {
public:
    Text(float x, float y,
         const std::string& content,
         float fontSize = 24.0f,
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
        // Center-based scaling: adjust position as font size changes
        float oldFontSize = fontSize_;
        setFontSize(w);
        
        if (oldFontSize > 0) {
            float fontScale = w / oldFontSize;
            
            float oldWidth = content_.length() * oldFontSize * 0.6f;
            float oldHeight = oldFontSize * 0.8f;
            
            float newWidth = oldWidth * fontScale;
            float newHeight = oldHeight * fontScale;
            
            float widthDiff = newWidth - oldWidth;
            float heightDiff = newHeight - oldHeight;
            
            x_ = centerX_ - newWidth * 0.5f;
            y_ = centerY_ + newHeight * 0.3f;

            x_ = centerX_;
            y_ = centerY_;
        }
    }
    
    void resetForAnimation() override {
        float textWidth = content_.length() * fontSize_ * 0.6f;
        float textHeight = fontSize_ * 0.8f;
        
        centerX_ = x_ + textWidth * 0.5f;
        centerY_ = y_ - textHeight * 0.3f;
    }

private:
    std::string content_;
    float fontSize_;
    float duration_;
    float centerX_ = 0, centerY_ = 0;
    void initDefaultSpawn(float duration) override;
};

} // namespace banim
