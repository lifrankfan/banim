#include "banim/shapes.h"
#include "banim/animations.h"
#include <cmath>
#include <memory>

namespace banim {

// ────────────── RECTANGLE ──────────────

Rectangle::Rectangle(float x, float y, float width, float height, float duration,
                     float r, float g, float b, float a, float rotation)
    : x_(x), y_(y), w_(width), h_(height), r_(r), g_(g), b_(b), rotation_(rotation) {
    a_ = a;
    initDefaultSpawn(duration);
}

void Rectangle::initDefaultSpawn(float duration) {
    auto pop = std::make_shared<PopIn>(std::shared_ptr<Shape>(this, [](Shape*){}), duration);
    spawnAnim_ = pop;
}

Rectangle &Rectangle::setColor(float r, float g, float b, float a) {
    r_ = r; g_ = g; b_ = b; a_ = a;
    return *this;
}

Rectangle &Rectangle::setRotation(float rotation) {
    rotation_ = rotation;
    return *this;
}

void Rectangle::setPos(float x, float y) {
    x_ = x;
    y_ = y;
}

void Rectangle::setSize(float width, float height) {
    w_ = width;
    h_ = height;
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

// ────────────── CIRCLE ──────────────

Circle::Circle(float cx, float cy, float rx, float ry, float duration,
               float r, float g, float b, float a, float rotation)
    : cx_(cx), cy_(cy), rx_(rx), ry_(ry), r_(r), g_(g), b_(b), rotation_(rotation) {
    a_ = a;
    initDefaultSpawn(duration);
}

void Circle::initDefaultSpawn(float duration) {
    auto pop = std::make_shared<PopIn>(std::shared_ptr<Shape>(this, [](Shape*){}), duration);
    spawnAnim_ = pop;
}

Circle &Circle::setColor(float r, float g, float b, float a) {
    r_ = r; g_ = g; b_ = b; a_ = a;
    return *this;
}

Circle &Circle::setRotation(float rotation) {
    rotation_ = rotation;
    return *this;
}

void Circle::setPos(float x, float y) {
    cx_ = x;
    cy_ = y;
}

void Circle::setSize(float rx, float ry) {
    rx_ = rx;
    ry_ = ry;
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
