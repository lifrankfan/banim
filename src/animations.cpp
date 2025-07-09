#include "banim/animations.h"
#include <banim/shapes.h>
#include <cmath>
#include <algorithm>

#include <iostream>

namespace banim {

    PopIn::PopIn(std::shared_ptr<Shape> shape, float duration)
    : shape_(shape), duration_(duration) {
    if (auto rect = std::dynamic_pointer_cast<Rectangle>(shape_)) {
        targetW_ = rect->getWidth();
        targetH_ = rect->getHeight();
        // rect->resize(0.0f, 0.0f); 
    } else if (auto circ = std::dynamic_pointer_cast<Circle>(shape_)) {
        targetW_ = circ->getRx();
        targetH_ = circ->getRy();
        // circ->resize(0.0f, 0.0f);
    }
}

bool PopIn::update(float dt) {
    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t >= 1.0f) t = 1.0f;

    float scale = t;  // linear

    if (!started_) {
        shape_->show();
    started_ = true;
}


    if (auto rect = std::dynamic_pointer_cast<Rectangle>(shape_)) {
        rect->setSize(targetW_ * scale, targetH_ * scale);
    } else if (auto circ = std::dynamic_pointer_cast<Circle>(shape_)) {
        circ->setSize(targetW_ * scale, targetH_ * scale);
    }

    

    return t < 1.0f;
}

MoveTo::MoveTo(std::shared_ptr<Shape> target, float toX, float toY, float duration)
    : target_(target), toX_(toX), toY_(toY), duration_(duration) {}

bool MoveTo::update(float dt) {
    if (!initialized_) {
        fromX_ = target_->x();
        fromY_ = target_->y();
        initialized_ = true;
    }
    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f)
        t = 1.0f;

    float newX = fromX_ + (toX_ - fromX_) * t;
    float newY = fromY_ + (toY_ - fromY_) * t;
    target_->setPos(newX, newY);

    return t < 1.0f;
}

ResizeTo::ResizeTo(std::shared_ptr<Shape> shape, float targetW, float targetH, float duration)
    : shape_(shape), targetW_(targetW), targetH_(targetH), duration_(duration) {}

bool ResizeTo::update(float dt) {
    if (!initialized_) {
        if (auto rect = std::dynamic_pointer_cast<Rectangle>(shape_)) {
            startW_ = rect->getWidth();
            startH_ = rect->getHeight();
        } else if (auto circ = std::dynamic_pointer_cast<Circle>(shape_)) {
            startW_ = circ->getRx();
            startH_ = circ->getRy();
        }
        initialized_ = true;
    }

    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f) t = 1.0f;

    float newW = startW_ + (targetW_ - startW_) * t;
    float newH = startH_ + (targetH_ - startH_) * t;
    shape_->setSize(newW, newH);

    return t < 1.0f;
}


Wait::Wait(float duration) : duration_(duration) {}

bool Wait::update(float dt) {
    elapsed_ += dt;
    return elapsed_ < duration_;
}


} // namespace banim