#include "banim/text.h"
#include "banim/animations.h"
#include "banim/scene.h"

namespace banim {

Text::Text(float x, float y,
           const std::string& content,
           float fontSize,
           float duration)
    : content_(content), fontSize_(fontSize), duration_(duration)
{
    x_ = x;
    y_ = y;
}

Text::Text(const GridCoord& gridPos,
           const std::string& content,
           float fontSize,
           const Scene* scene,
           float duration)
    : content_(content), fontSize_(fontSize), duration_(duration)
{
    if (scene) {
        auto [pixelX, pixelY] = scene->gridToPixel(gridPos);
        
        // Calculate text dimensions for centering
        float textWidth = content_.length() * fontSize_ * 0.6f;  // Rough estimation
        float textHeight = fontSize_;
        
        // Position text so its visual center is at the grid position
        x_ = pixelX - textWidth * 0.5f;
        y_ = pixelY + textHeight * 0.3f;  // Adjust for baseline positioning
    } else {
        x_ = gridPos.x;
        y_ = gridPos.y;
    }
}

void Text::setText(const std::string& content) { content_ = content; }
void Text::setFontSize(float size) { fontSize_ = size; }

void Text::draw(cairo_t* cr) {
    cairo_save(cr);

    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    cairo_select_font_face(cr, "Sans",
                           CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, fontSize_);

    cairo_move_to(cr, x_, y_);
    cairo_show_text(cr, content_.c_str());

    cairo_restore(cr);
}

void Text::setSize(float w, float h) {
}

} // namespace banim
