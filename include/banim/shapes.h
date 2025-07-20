#pragma once

#include <cairo/cairo.h>
#include <memory>

namespace banim {

class Animation;

class Shape : public std::enable_shared_from_this<Shape> {
  public:
    virtual ~Shape() = default;
    virtual void draw(cairo_t* cr) = 0;
    virtual float x() const { return x_; }
    virtual float y() const { return y_; }
    virtual void setPos(float x, float y) { x_ = x; y_ = y; }
    virtual void setSize(float w, float h) = 0;

    virtual void setAlpha(float alpha) { a_ = alpha; }
    virtual float getAlpha() const { return a_; }
    virtual void hide() { setAlpha(0.0f); }
    virtual void show() { setAlpha(1.0f); }

    virtual Shape& setColor(float r, float g, float b, float a = 1.0f) {
        r_ = r; g_ = g; b_ = b; a_ = a;
        return *this;
    }

    virtual Shape& setRotation(float angle) {
        rotation_ = angle;
        return *this;
    }

    virtual Shape& setFilled(bool filled) {
        filled_ = filled;
        return *this;
    }

    virtual bool isFilled() const { return filled_; }

    virtual Shape& setStrokeWidth(float w) {
        strokeWidth_ = w;
        return *this;
    }

    virtual float getStrokeWidth() const { return strokeWidth_; }

    // Animation interface methods
    virtual void getAnimatableSize(float& w, float& h) const = 0;
    virtual void setAnimatableSize(float w, float h) = 0;
    virtual void resetForAnimation() = 0;

  protected:
    float x_ = 0, y_ = 0;
    float r_ = 0, g_ = 0, b_ = 0, a_ = 1;
    float rotation_ = 0;
    float strokeWidth_ = 2.0f;
    bool filled_ = true;
};

class Rectangle : public Shape {
  public:
    Rectangle(float x, float y, float width, float height,
              float duration = 0.5f,
              float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f,
              float rotation = 0.0f);

    Rectangle& setBorderRadius(float radius);
    float getBorderRadius() const { return borderRadius_; }

    void setPos(float x, float y) override;
    void setSize(float w, float h) override;

    float getWidth() const { return w_; }
    float getHeight() const { return h_; }

    void draw(cairo_t* cr) override;

    void getAnimatableSize(float& w, float& h) const override {
        w = w_;
        h = h_;
    }
    
    void setAnimatableSize(float w, float h) override {
        // Center-based scaling: keep center point fixed
        float centerX = x_ + w_ / 2.0f;
        float centerY = y_ + h_ / 2.0f;
        
        // Update size
        w_ = w;
        h_ = h;
        
        // Adjust position to keep center fixed
        x_ = centerX - w_ / 2.0f;
        y_ = centerY - h_ / 2.0f;
    }
    
    void resetForAnimation() override {
        // Store the original center point for animation
        centerX_ = x_ + w_ / 2.0f;
        centerY_ = y_ + h_ / 2.0f;
    }

  private:
    float w_ = 0, h_ = 0;
    float borderRadius_ = 0.0f;
    float duration_;
    float centerX_ = 0, centerY_ = 0; // For center-based animation scaling
};

class Circle : public Shape {
  public:
    Circle(float cx, float cy, float rx, float ry,
           float duration = 0.5f,
           float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f,
           float rotation = 0.0f);

    void setPos(float x, float y) override;
    void setSize(float rx, float ry) override;

    float getRx() const { return rx_; }
    float getRy() const { return ry_; }

    void draw(cairo_t* cr) override;

    void getAnimatableSize(float& w, float& h) const override {
        w = rx_;
        h = ry_;
    }
    
    void setAnimatableSize(float w, float h) override {
        // Circles are already center-based (x_, y_ is the center)
        setSize(w, h);
    }
    
    void resetForAnimation() override {
        // Circles don't need special center tracking since they're already centered
    }

  private:
    float rx_ = 0, ry_ = 0;
    float duration_;
};

} // namespace banim
