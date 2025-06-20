#pragma once

#include <memory>
#include "banim/scene.h"
#include "banim/shapes.h"

namespace banim {

class Animation {
  public:
    virtual ~Animation() = default;
    virtual bool update(float dt) = 0;
};

class PopIn : public Animation {
  public:
    PopIn(std::shared_ptr<Shape> shape, float duration, float overshoot = 1.2f);
    bool update(float dt) override;
    void setFinalSize(float w, float h);

  private:
    std::shared_ptr<Shape> shape_;
    float elapsed_ = 0.0f;
    float duration_;
    float overshoot_;
    float targetW_ = -1.0f, targetH_ = -1.0f;

    float easeOutBack(float t, float overshoot);
};

class MoveTo : public Animation {
  public:
    MoveTo(std::shared_ptr<Shape> target, float toX, float toY, float duration);
    bool update(float dt) override;

  private:
    std::shared_ptr<Shape> target_;
    float fromX_, fromY_;
    float toX_, toY_;
    float duration_;
    float elapsed_ = 0.0f;
    bool initialized_ = false;
};

} // namespace banim