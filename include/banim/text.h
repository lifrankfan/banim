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

private:
    std::string content_;
    float fontSize_;
    float duration_;
    void initDefaultSpawn(float duration) override;
};

} // namespace banim
