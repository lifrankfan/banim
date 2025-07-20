#include "banim/shapes.h"
#include "banim/animations.h"
#include <cmath>
#include <memory>

namespace banim {

// ────────────── RECTANGLE ──────────────

Rectangle::Rectangle(float x, float y, float width, float height,
                     float duration, float r, float g, float b, float a,
                     float rotation) : duration_(duration) {
    x_ = x; y_ = y;
    w_ = width; h_ = height;
    r_ = r; g_ = g; b_ = b; a_ = a;
    rotation_ = rotation;
}

Rectangle& Rectangle::setBorderRadius(float radius) {
    borderRadius_ = radius;
    return *this;
}

void Rectangle::setPos(float x, float y) {
    x_ = x;
    y_ = y;
}

void Rectangle::setSize(float w, float h) {
    w_ = w;
    h_ = h;
}

void Rectangle::draw(cairo_t* cr) {
    cairo_save(cr);
    cairo_translate(cr, x_ + w_ / 2, y_ + h_ / 2);
    cairo_rotate(cr, rotation_);
    cairo_translate(cr, -w_ / 2, -h_ / 2);

    if (borderRadius_ > 0.0f) {
        float r = std::min(borderRadius_, std::min(w_ / 2, h_ / 2));
        cairo_new_sub_path(cr);
        cairo_arc(cr, w_ - r, r, r, -M_PI_2, 0);
        cairo_arc(cr, w_ - r, h_ - r, r, 0, M_PI_2);
        cairo_arc(cr, r, h_ - r, r, M_PI_2, M_PI);
        cairo_arc(cr, r, r, r, M_PI, 3 * M_PI_2);
        cairo_close_path(cr);
    } else {
        cairo_rectangle(cr, 0, 0, w_, h_);
    }

    cairo_set_line_width(cr, strokeWidth_);
    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    if (filled_)
        cairo_fill(cr);
    else
        cairo_stroke(cr);

    cairo_restore(cr);
}

// ────────────── CIRCLE ──────────────

Circle::Circle(float cx, float cy, float rx, float ry,
               float duration, float r, float g, float b, float a,
               float rotation) : duration_(duration) {
    x_ = cx; y_ = cy;
    rx_ = rx; ry_ = ry;
    r_ = r; g_ = g; b_ = b; a_ = a;
    rotation_ = rotation;
}

void Circle::setPos(float x, float y) {
    x_ = x;
    y_ = y;
}

void Circle::setSize(float rx, float ry) {
    rx_ = rx;
    ry_ = ry;
}

void Circle::draw(cairo_t *cr) {
    cairo_save(cr);
    cairo_translate(cr, x_, y_);
    cairo_rotate(cr, rotation_);

    cairo_save(cr);
    cairo_scale(cr, rx_, ry_);
    cairo_arc(cr, 0, 0, 1.0, 0, 2 * M_PI);
    cairo_restore(cr);

    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    if (filled_) {
        cairo_fill(cr);
    } else {
        cairo_set_line_width(cr, strokeWidth_);
        cairo_stroke(cr);
    }

    cairo_restore(cr);
}

} // namespace banim
