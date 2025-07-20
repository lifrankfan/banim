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
        setFontSize(w);
    }
    
    void resetForAnimation() override {
        // Text doesn't need special reset logic for most animations
    }

private:
    std::string content_;
    float fontSize_;
    float duration_;
    void initDefaultSpawn(float duration) override;
};

} // namespace banim
