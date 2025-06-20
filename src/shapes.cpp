#include "banim/shapes.h"
#include <cmath>
#include <memory>

namespace banim {

Rectangle::Rectangle(float x, float y, float width, float height, float r,
                     float g, float b, float a, float rotation)
    : x_(x), y_(y), w_(width), h_(height), r_(r), g_(g), b_(b), a_(a),
      rotation_(rotation) {}

Rectangle &Rectangle::setColor(float r, float g, float b, float a) {
    r_ = r;
    g_ = g;
    b_ = b;
    a_ = a;
    return *this;
}

Rectangle &Rectangle::setRotation(float rotation) {
    rotation_ = rotation;
    return *this;
}

Rectangle &Rectangle::moveTo(float x, float y) {
    x_ = x;
    y_ = y;
    return *this;
}

Rectangle &Rectangle::resize(float width, float height) {
    w_ = width;
    h_ = height;
    return *this;
}

void Rectangle::draw(cairo_t *cr) {
    cairo_save(cr);
    cairo_translate(cr, x_ + w_ / 2, y_ + h_ / 2);
    cairo_rotate(cr, rotation_);
    cairo_translate(cr, -w_ / 2, -h_ / 2);
    cairo_rectangle(cr, 0, 0, w_, h_);
    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    cairo_fill(cr);
    cairo_restore(cr);
}

Circle::Circle(float cx, float cy, float rx, float ry, float r, float g,
               float b, float a, float rotation)
    : cx_(cx), cy_(cy), rx_(rx), ry_(ry), r_(r), g_(g), b_(b), a_(a),
      rotation_(rotation) {}

Circle &Circle::setColor(float r, float g, float b, float a) {
    r_ = r;
    g_ = g;
    b_ = b;
    a_ = a;
    return *this;
}

Circle &Circle::setRotation(float rotation) {
    rotation_ = rotation;
    return *this;
}

Circle &Circle::moveTo(float cx, float cy) {
    cx_ = cx;
    cy_ = cy;
    return *this;
}

Circle &Circle::resize(float rx, float ry) {
    rx_ = rx;
    ry_ = ry;
    return *this;
}

void Circle::draw(cairo_t *cr) {
    cairo_save(cr);
    cairo_translate(cr, cx_, cy_);
    cairo_rotate(cr, rotation_);
    cairo_scale(cr, rx_, ry_);
    cairo_arc(cr, 0, 0, 1.0, 0, 2 * M_PI);
    cairo_set_source_rgba(cr, r_, g_, b_, a_);
    cairo_fill(cr);
    cairo_restore(cr);
}

} // namespace banim
