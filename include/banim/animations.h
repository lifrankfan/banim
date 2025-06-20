#pragma once

#include "banim/scene.h"
#include "banim/shapes.h"
#include <memory>

namespace banim {

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

} // namespace banim
