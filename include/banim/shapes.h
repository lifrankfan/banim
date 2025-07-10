#pragma once

#include <cairo/cairo.h>
#include <memory>

namespace banim {

class Animation;

class Shape {
  public:
    virtual ~Shape() = default;
    virtual void draw(cairo_t *cr) = 0;
    virtual float x() const = 0;
    virtual float y() const = 0;
    virtual void setPos(float x, float y) = 0;
    virtual void setSize(float w, float h) = 0;

    // Opacity interface
    virtual void setAlpha(float alpha) { a_ = alpha; }
    virtual float getAlpha() const { return a_; }
    virtual void hide() { setAlpha(0.0f); }
    virtual void show() { setAlpha(1.0f); }

    virtual Shape& setStrokeWidth(float w) { strokeWidth_ = w; return *this; }
    virtual float getStrokeWidth() const { return strokeWidth_; }

    std::shared_ptr<Animation> spawnAnimation() const { return spawnAnim_; }
    void setSpawnAnimation(std::shared_ptr<Animation> anim) {
        spawnAnim_ = anim;
    }

  protected:
    std::shared_ptr<Animation> spawnAnim_ = nullptr;
    virtual void initDefaultSpawn(float duration) = 0;

    float a_ = 1.0f;
    float strokeWidth_ = 2.0f;
};

class Rectangle : public Shape {
  public:
    Rectangle(float x, float y, float width, float height,
              float duration = 0.5f,
              float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f,
              float rotation = 0.0f);

    Rectangle& setColor(float r, float g, float b, float a);
    Rectangle& setRotation(float rotation);
    Rectangle& setFilled(bool filled);
    Rectangle& setBorderRadius(float radius);
    float getBorderRadius() const { return borderRadius_; }
    Rectangle& setStrokeWidth(float w) override {
        strokeWidth_ = w;
        return *this;
    }



    float x() const override { return x_; }
    float y() const override { return y_; }

    void setPos(float x, float y) override;
    void setSize(float w, float h) override;

    float getWidth() const { return w_; }
    float getHeight() const { return h_; }

    void draw(cairo_t *cr) override;

  private:
    float x_, y_, w_, h_;
    float r_, g_, b_;
    float a_;
    float rotation_;
    bool filled_ = true;
    float borderRadius_ = 0.0f;

    void initDefaultSpawn(float duration) override;
};

class Circle : public Shape {
  public:
    Circle(float cx, float cy, float rx, float ry, float duration = 0.5f,
           float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f,
           float rotation = 0.0f);

    Circle &setColor(float r, float g, float b, float a);
    Circle &setRotation(float rotation);

    Circle &setFilled(bool filled);
    bool isFilled() const { return filled_; }
    Circle& setStrokeWidth(float w) override {
    strokeWidth_ = w;
    return *this;
}

    float x() const override { return cx_; }
    float y() const override { return cy_; }
    void setPos(float x, float y) override;
    void setSize(float rx, float ry) override;

    float getRx() const { return rx_; }
    float getRy() const { return ry_; }

    void draw(cairo_t *cr) override;

  private:
    float cx_, cy_, rx_, ry_;
    float r_, g_, b_;
    float rotation_;
    bool filled_ = true; 

    void initDefaultSpawn(float duration) override;
};

} // namespace banim
