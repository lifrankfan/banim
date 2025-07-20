#include "banim/text.h"
#include "banim/animations.h"

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

void Text::initDefaultSpawn(float duration) {
    spawnAnim_ = std::make_shared<PopIn>(shared_from_this(), duration_);
}

} // namespace banim
