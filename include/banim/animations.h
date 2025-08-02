#pragma once

#include <memory>
#include "banim/grid.h"

#define default_duration 0.5f

namespace banim {

class Scene;
class Shape;
class Rectangle;

class Animation {
  public:
    virtual ~Animation() = default;
    virtual bool update(float dt) = 0;
};

class PopIn : public Animation {
  public:
    PopIn(std::shared_ptr<Shape> shape, float duration = default_duration);

    bool update(float dt) override;

  private:
    std::shared_ptr<Shape> shape_;
    float elapsed_ = 0.0f;
    float duration_;
    bool started_ = false;
    float targetW_ = -1.0f, targetH_ = -1.0f;
};


class MoveTo : public Animation {
  public:
    // Pixel-based movement
    MoveTo(std::shared_ptr<Shape> target, float toX, float toY, float duration = default_duration);
    
    // Grid-based movement  
    MoveTo(std::shared_ptr<Shape> target, const GridCoord& toGrid, const Scene* scene, float duration = default_duration);
    
    bool update(float dt) override;

  private:
    std::shared_ptr<Shape> target_;
    float fromX_, fromY_;
    float toX_, toY_;
    float duration_;
    float elapsed_ = 0.0f;
    bool initialized_ = false;
};

class ResizeTo : public Animation {
  public:
    // Pixel-based resizing
    ResizeTo(std::shared_ptr<Shape> shape, float targetW, float targetH, float duration = default_duration);
    
    // Grid-based resizing
    ResizeTo(std::shared_ptr<Shape> shape, float gridW, float gridH, const Scene* scene, float duration = default_duration);
    
    bool update(float dt) override;

  private:
    std::shared_ptr<Shape> shape_;
    float startW_ = 0.0f, startH_ = 0.0f;
    float targetW_, targetH_;
    float duration_;
    float elapsed_ = 0.0f;
    bool initialized_ = false;
};

class BorderTo : public Animation {
  public:
    BorderTo(std::shared_ptr<Rectangle> rect, float targetRadius, float duration = default_duration);
    bool update(float dt) override;

  private:
    std::shared_ptr<Rectangle> rect_;
    float startRadius_ = 0.0f;
    float targetRadius_;
    float duration_;
    float elapsed_ = 0.0f;
    bool initialized_ = false;
};

class Wait : public Animation {
  public:
    explicit Wait(float duration);
    bool update(float dt) override;

  private:
    float duration_;
    float elapsed_ = 0.0f;
};

class StrokeTo : public Animation {
  public:
    StrokeTo(std::shared_ptr<Shape> shape, float targetStroke, float duration);

    bool update(float dt) override;

  private:
    std::shared_ptr<Shape> shape_;
    float startStroke_;
    float targetStroke_;
    float duration_;
    float elapsed_ = 0.0f;
};

} // namespace banim