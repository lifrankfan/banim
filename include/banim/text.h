#pragma once
#include "banim/shapes.h"
#include <string>
#include <cairo/cairo.h>

namespace banim {

class Text : public Shape {
public:
    Text(float x, float y,
         const std::string& content,
         float fontSize = 24.0f);

    void setText(const std::string& content);
    void setFontSize(float size);
    void draw(cairo_t* cr) override;
    void setSize(float w, float h) override;

private:
    std::string content_;
    float fontSize_;
    void initDefaultSpawn(float duration) override;
};

} // namespace banim
