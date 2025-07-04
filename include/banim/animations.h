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
    PopIn(std::shared_ptr<Shape> shape, float duration = 1.0f);

    bool update(float dt) override;

  private:
    std::shared_ptr<Shape> shape_;
    float elapsed_ = 0.0f;
    float duration_;

    float targetW_ = -1.0f, targetH_ = -1.0f;
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

class Wait : public Animation {
  public:
    explicit Wait(float duration);
    bool update(float dt) override;

  private:
    float duration_;
    float elapsed_ = 0.0f;
};


} // namespace banim