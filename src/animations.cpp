#include "banim/animations.h"
#include <banim/shapes.h>
#include <banim/text.h>
#include <cmath>
#include <algorithm>

#include <iostream>

namespace banim {

PopIn::PopIn(std::shared_ptr<Shape> shape, float duration)
    : shape_(shape), duration_(duration) {
    shape_->getAnimatableSize(targetW_, targetH_);
    shape_->resetForAnimation();
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

    shape_->setAnimatableSize(targetW_ * scale, targetH_ * scale);

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
        shape_->getAnimatableSize(startW_, startH_);
        initialized_ = true;
    }

    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f) t = 1.0f;

    float newW = startW_ + (targetW_ - startW_) * t;
    float newH = startH_ + (targetH_ - startH_) * t;
    
    shape_->setAnimatableSize(newW, newH);

    return t < 1.0f;
}

BorderTo::BorderTo(std::shared_ptr<Rectangle> rect, float targetRadius, float duration)
    : rect_(rect), targetRadius_(targetRadius), duration_(duration) {}

bool BorderTo::update(float dt) {
    if (!initialized_) {
        startRadius_ = rect_->getBorderRadius();
        initialized_ = true;
    }

    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f) t = 1.0f;

    // Ease-in-out interpolation
    t = t * t * (3 - 2 * t);

    float newRadius = startRadius_ + (targetRadius_ - startRadius_) * t;
    rect_->setBorderRadius(newRadius);

    return t < 1.0f;
}


StrokeTo::StrokeTo(std::shared_ptr<Shape> shape, float targetStroke, float duration)
    : shape_(shape), targetStroke_(targetStroke), duration_(duration) {
    startStroke_ = shape_->getStrokeWidth();
}

bool StrokeTo::update(float dt) {
    elapsed_ += dt;
    float t = std::min(elapsed_ / duration_, 1.0f);
    float newStroke = startStroke_ + t * (targetStroke_ - startStroke_);
    shape_->setStrokeWidth(newStroke);
    return t < 1.0f;
}

Wait::Wait(float duration) : duration_(duration) {}

bool Wait::update(float dt) {
    elapsed_ += dt;
    return elapsed_ < duration_;
}


} // namespace banim