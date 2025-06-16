#pragma once

#include <cairo.h>
#include <memory>

namespace banim {

class Shape {
  public:
    Shape() = default;
    virtual ~Shape() = default;
    virtual void draw(cairo_t *cr) = 0;
};

class Rectangle : public Shape {
  public:
    Rectangle(float x, float y, float width, float height, float r = 0.0f,
              float g = 0.0f, float b = 0.0f, float a = 1.0f,
              float rotation = 0.0f);
    void draw(cairo_t *cr) override;

    Rectangle &setColor(float r, float g, float b, float a);
    Rectangle &setRotation(float rotation);
    Rectangle &moveTo(float x, float y);
    Rectangle &resize(float width, float height);

  private:
    float x_, y_, w_, h_;
    float r_, g_, b_, a_;
    float rotation_;
};

class Circle : public Shape {
  public:
    Circle(float cx, float cy, float rx, float ry, float r = 0.0f,
           float g = 0.0f, float b = 0.0f, float a = 1.0f,
           float rotation = 0.0f);
    void draw(cairo_t *cr) override;

    Circle &setColor(float r, float g, float b, float a);
    Circle &setRotation(float rotation);
    Circle &moveTo(float cx, float cy);
    Circle &resize(float rx, float ry);

  private:
    float cx_, cy_, rx_, ry_;
    float r_, g_, b_, a_;
    float rotation_;
};

} // namespace banim
