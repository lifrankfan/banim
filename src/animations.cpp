#include "banim/animations.h"
#include <banim/shapes.h>
#include <cmath>

namespace banim {

PopIn::PopIn(std::shared_ptr<Shape> shape, float duration, float overshoot)
    : shape_(shape), duration_(duration), overshoot_(overshoot) {}

void PopIn::setFinalSize(float w, float h) {
    targetW_ = w;
    targetH_ = h;
}

float PopIn::easeOutBack(float t, float overshoot) {
    float s = overshoot;
    return 1 + s * std::pow(t - 1, 3) + s * (t - 1) * std::pow(t - 1, 2);
}

bool PopIn::update(float dt) {
    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t >= 1.0f)
        t = 1.0f;

    float s = easeOutBack(t, overshoot_);

    if (auto rect = std::dynamic_pointer_cast<Rectangle>(shape_)) {
        float baseW = targetW_ < 0 ? rect->getWidth() : targetW_;
        float baseH = targetH_ < 0 ? rect->getHeight() : targetH_;
        rect->resize(baseW * s, baseH * s);
    } else if (auto circ = std::dynamic_pointer_cast<Circle>(shape_)) {
        float baseRx = targetW_ < 0 ? circ->getRx() : targetW_;
        float baseRy = targetH_ < 0 ? circ->getRy() : targetH_;
        circ->resize(baseRx * s, baseRy * s);
    }

    return t < 1.0f;
}

} // namespace banim
