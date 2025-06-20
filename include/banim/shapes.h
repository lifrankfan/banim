#pragma once

#include <cairo/cairo.h>
#include <memory>

namespace banim {

class Animation; // forward declaration

class Shape {
  public:
    virtual ~Shape() = default;
    virtual void draw(cairo_t *cr) = 0;
    virtual float x() const = 0;
    virtual float y() const = 0;
    virtual void setPos(float x, float y) = 0;

    std::shared_ptr<Animation> spawnAnimation() const { return spawnAnim_; }
    void setSpawnAnimation(std::shared_ptr<Animation> anim) {
        spawnAnim_ = anim;
    }

  protected:
    std::shared_ptr<Animation> spawnAnim_;
};

class Rectangle : public Shape {
  public:
    Rectangle(float x, float y, float width, float height, float r = 0.0f,
              float g = 0.0f, float b = 0.0f, float a = 1.0f,
              float rotation = 0.0f);

    Rectangle &setColor(float r, float g, float b, float a);
    Rectangle &setRotation(float rotation);
    Rectangle &moveTo(float x, float y);
    Rectangle &resize(float width, float height);

    float x() const override { return x_; }
    float y() const override { return y_; }
    void setPos(float x, float y) override {
        x_ = x;
        y_ = y;
    }

    float getWidth() const { return w_; }
    float getHeight() const { return h_; }

    void draw(cairo_t *cr) override;

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

    Circle &setColor(float r, float g, float b, float a);
    Circle &setRotation(float rotation);
    Circle &moveTo(float cx, float cy);
    Circle &resize(float rx, float ry);

    float x() const override { return cx_; }
    float y() const override { return cy_; }
    void setPos(float x, float y) override {
        cx_ = x;
        cy_ = y;
    }

    float getRx() const { return rx_; }
    float getRy() const { return ry_; }

    void draw(cairo_t *cr) override;

  private:
    float cx_, cy_, rx_, ry_;
    float r_, g_, b_, a_;
    float rotation_;
};

} // namespace banim
