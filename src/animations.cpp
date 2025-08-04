#include "banim/animations.h"
#include <banim/animatable.h>
#include <banim/scene.h>
#include <cmath>
#include <algorithm>

#include <iostream>

namespace banim {

PopIn::PopIn(std::shared_ptr<Animatable> animatable, float duration)
    : animatable_(animatable), duration_(duration) {
    animatable_->getAnimatableSize(targetW_, targetH_);
    animatable_->resetForAnimation();
}

bool PopIn::update(float dt) {
    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t >= 1.0f) t = 1.0f;

    float scale = t;  // linear

    if (!started_) {
        animatable_->show();
        started_ = true;
    }

    animatable_->setAnimatableSize(targetW_ * scale, targetH_ * scale);

    return t < 1.0f;
}

MoveTo::MoveTo(std::shared_ptr<Animatable> target, const GridCoord& toGrid, float duration)
    : target_(target), toGrid_(toGrid), duration_(duration) {}

bool MoveTo::update(float dt) {
    if (!initialized_) {
        fromGrid_ = target_->getGridPos();
        initialized_ = true;
    }
    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f)
        t = 1.0f;

    float newX = fromGrid_.x + (toGrid_.x - fromGrid_.x) * t;
    float newY = fromGrid_.y + (toGrid_.y - fromGrid_.y) * t;
    target_->setGridPos(newX, newY);

    return t < 1.0f;
}

ResizeTo::ResizeTo(std::shared_ptr<Animatable> animatable, float gridW, float gridH, float duration)
    : animatable_(animatable), targetW_(gridW), targetH_(gridH), duration_(duration) {}

bool ResizeTo::update(float dt) {
    if (!initialized_) {
        animatable_->getAnimatableSize(startW_, startH_);
        initialized_ = true;
    }

    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f) t = 1.0f;

    float newW = startW_ + (targetW_ - startW_) * t;
    float newH = startH_ + (targetH_ - startH_) * t;
    
    animatable_->setAnimatableSize(newW, newH);

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


StrokeTo::StrokeTo(std::shared_ptr<Animatable> animatable, float targetStroke, float duration)
    : animatable_(animatable), targetStroke_(targetStroke), duration_(duration) {
    startStroke_ = animatable_->getStrokeWidth();
}

bool StrokeTo::update(float dt) {
    elapsed_ += dt;
    float t = std::min(elapsed_ / duration_, 1.0f);
    float newStroke = startStroke_ + t * (targetStroke_ - startStroke_);
    animatable_->setStrokeWidth(newStroke);
    return t < 1.0f;
}

Wait::Wait(float duration) : duration_(duration) {}

bool Wait::update(float dt) {
    elapsed_ += dt;
    return elapsed_ < duration_;
}

MoveWaypoint::MoveWaypoint(std::shared_ptr<Line> line, int waypointIndex, const GridCoord& targetPos, float duration)
    : line_(line), waypointIndex_(waypointIndex), targetPos_(targetPos), duration_(duration) {}

bool MoveWaypoint::update(float dt) {
    if (!initialized_) {
        if (waypointIndex_ >= 0 && waypointIndex_ < line_->getWaypointCount()) {
            startPos_ = line_->getWaypoint(waypointIndex_);
            initialized_ = true;
        } else {
            return false; // Invalid waypoint index
        }
    }
    
    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f) t = 1.0f;
    
    float newX = startPos_.x + (targetPos_.x - startPos_.x) * t;
    float newY = startPos_.y + (targetPos_.y - startPos_.y) * t;
    line_->setWaypoint(waypointIndex_, {newX, newY});
    
    return t < 1.0f;
}

MoveLineEnd::MoveLineEnd(std::shared_ptr<Line> line, const GridCoord& targetEndPos, float duration)
    : line_(line), targetEndPos_(targetEndPos), duration_(duration) {}

bool MoveLineEnd::update(float dt) {
    if (!initialized_) {
        startEndPos_ = line_->getEndPos();
        initialized_ = true;
    }
    
    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f) t = 1.0f;
    
    float newX = startEndPos_.x + (targetEndPos_.x - startEndPos_.x) * t;
    float newY = startEndPos_.y + (targetEndPos_.y - startEndPos_.y) * t;
    line_->setEndPos({newX, newY});
    
    return t < 1.0f;
}

MoveLineStart::MoveLineStart(std::shared_ptr<Line> line, const GridCoord& targetStartPos, float duration)
    : line_(line), targetStartPos_(targetStartPos), duration_(duration) {}

bool MoveLineStart::update(float dt) {
    if (!initialized_) {
        startStartPos_ = line_->getGridPos();
        initialized_ = true;
    }
    
    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f) t = 1.0f;
    
    float newX = startStartPos_.x + (targetStartPos_.x - startStartPos_.x) * t;
    float newY = startStartPos_.y + (targetStartPos_.y - startStartPos_.y) * t;
    
    // Move only the start position, keeping end and waypoints in place
    float deltaX = newX - line_->getGridPos().x;
    float deltaY = newY - line_->getGridPos().y;
    
    // Update the base grid position directly (just the start point)
    line_->Animatable::setGridPos(newX, newY);
    
    return t < 1.0f;
}

RemoveWaypoint::RemoveWaypoint(std::shared_ptr<Line> line, int waypointIndex, float duration)
    : line_(line), waypointIndex_(waypointIndex), duration_(duration) {}

bool RemoveWaypoint::update(float dt) {
    if (!initialized_) {
        // Store the original waypoint position
        if (waypointIndex_ >= 0 && waypointIndex_ < line_->getWaypointCount()) {
            originalWaypoint_ = line_->getWaypoint(waypointIndex_);
            
            // Calculate what the straight line position would be if this waypoint didn't exist
            // We need to find the position on the line connecting the waypoints before and after this one
            GridCoord beforePos, afterPos;
            
            if (waypointIndex_ == 0) {
                // First waypoint - interpolate between start and next waypoint
                beforePos = line_->getGridPos();
                if (line_->getWaypointCount() > 1) {
                    afterPos = line_->getWaypoint(1);
                } else {
                    afterPos = line_->getEndPos();
                }
            } else if (waypointIndex_ == line_->getWaypointCount() - 1) {
                // Last waypoint - interpolate between previous waypoint and end
                beforePos = line_->getWaypoint(waypointIndex_ - 1);
                afterPos = line_->getEndPos();
            } else {
                // Middle waypoint - interpolate between previous and next waypoints
                beforePos = line_->getWaypoint(waypointIndex_ - 1);
                afterPos = line_->getWaypoint(waypointIndex_ + 1);
            }
            
            // Target position is halfway between before and after
            targetPos_.x = (beforePos.x + afterPos.x) * 0.5f;
            targetPos_.y = (beforePos.y + afterPos.y) * 0.5f;
            
            initialized_ = true;
        } else {
            return false; // Invalid waypoint index
        }
    }

    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f) t = 1.0f;
    
    // Interpolate from original waypoint position to the target position
    GridCoord currentPos;
    currentPos.x = originalWaypoint_.x + (targetPos_.x - originalWaypoint_.x) * t;
    currentPos.y = originalWaypoint_.y + (targetPos_.y - originalWaypoint_.y) * t;
    
    line_->setWaypoint(waypointIndex_, currentPos);
    
    // Remove waypoint when animation completes
    if (t >= 1.0f) {
        line_->removeWaypoint(waypointIndex_);
    }
    
    return t < 1.0f;
}

ClearWaypoints::ClearWaypoints(std::shared_ptr<Line> line, float duration)
    : line_(line), duration_(duration) {}

bool ClearWaypoints::update(float dt) {
    if (!initialized_) {
        // Store original waypoints
        int count = line_->getWaypointCount();
        for (int i = 0; i < count; ++i) {
            originalWaypoints_.push_back(line_->getWaypoint(i));
        }
        initialized_ = true;
    }

    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f) t = 1.0f;

    // Gradually move all waypoints toward the line between start and end
    GridCoord startPos = line_->getGridPos();
    GridCoord endPos = line_->getEndPos();
    
    for (int i = 0; i < static_cast<int>(originalWaypoints_.size()); ++i) {
        // Calculate intermediate position on straight line
        float ratio = static_cast<float>(i + 1) / (originalWaypoints_.size() + 1);
        GridCoord targetPos;
        targetPos.x = startPos.x + (endPos.x - startPos.x) * ratio;
        targetPos.y = startPos.y + (endPos.y - startPos.y) * ratio;
        
        // Interpolate between original waypoint and target position
        GridCoord currentPos;
        currentPos.x = originalWaypoints_[i].x + (targetPos.x - originalWaypoints_[i].x) * t;
        currentPos.y = originalWaypoints_[i].y + (targetPos.y - originalWaypoints_[i].y) * t;
        
        line_->setWaypoint(i, currentPos);
    }
    
    // When animation completes, actually clear the waypoints
    if (t >= 1.0f) {
        line_->clearWaypoints();
        return false;
    }
    
    return true;
}

AddWaypoint::AddWaypoint(std::shared_ptr<Line> line, const GridCoord& newWaypoint, float duration)
    : line_(line), targetWaypoint_(newWaypoint), duration_(duration) {}

bool AddWaypoint::update(float dt) {
    if (!initialized_) {
        // Add the waypoint to the line and get its index
        line_->addWaypoint(targetWaypoint_);
        waypointIndex_ = line_->getWaypointCount() - 1;
        
        // Start from the line's end position for a smooth animation
        startPos_ = line_->getEndPos();
        
        // Set the new waypoint to start position initially
        line_->setWaypoint(waypointIndex_, startPos_);
        initialized_ = true;
    }

    elapsed_ += dt;
    float t = elapsed_ / duration_;
    if (t > 1.0f) t = 1.0f;

    // Interpolate between start position and target waypoint position
    GridCoord currentPos;
    currentPos.x = startPos_.x + (targetWaypoint_.x - startPos_.x) * t;
    currentPos.y = startPos_.y + (targetWaypoint_.y - startPos_.y) * t;
    
    line_->setWaypoint(waypointIndex_, currentPos);
    
    return t < 1.0f;
}

// AnimationGroup implementation
void AnimationGroup::add(std::shared_ptr<Animation> animation) {
    if (animation) {
        animations_.push_back(animation);
    }
}

void AnimationGroup::addAll(const std::vector<std::shared_ptr<Animation>>& animations) {
    for (auto& anim : animations) {
        if (anim) {
            animations_.push_back(anim);
        }
    }
}

bool AnimationGroup::update(float dt) {
    if (animations_.empty()) {
        return false; // No animations to update
    }
    
    // Update all animations and remove finished ones
    animations_.erase(
        std::remove_if(animations_.begin(), animations_.end(),
            [dt](std::shared_ptr<Animation>& anim) {
                return !anim->update(dt); // Remove if animation is finished
            }),
        animations_.end()
    );
    
    // Return true if there are still animations running
    return !animations_.empty();
}


} // namespace banim