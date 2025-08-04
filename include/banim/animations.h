#pragma once

#include <memory>
#include <vector>
#include "banim/grid.h"

#define default_duration 0.5f

namespace banim {

class Scene;
class Animatable;
class Rectangle;
class Line;

class Animation {
  public:
    virtual ~Animation() = default;
    virtual bool update(float dt) = 0;
};

class PopIn : public Animation {
  public:
    PopIn(std::shared_ptr<Animatable> animatable, float duration = default_duration);

    bool update(float dt) override;

  private:
    std::shared_ptr<Animatable> animatable_;
    float elapsed_ = 0.0f;
    float duration_;
    bool started_ = false;
    float targetW_ = -1.0f, targetH_ = -1.0f;
};

class MoveTo : public Animation {
  public:
    // Grid-based movement  
    MoveTo(std::shared_ptr<Animatable> target, const GridCoord& toGrid, float duration = default_duration);
    
    bool update(float dt) override;

  private:
    std::shared_ptr<Animatable> target_;
    GridCoord fromGrid_;
    GridCoord toGrid_;
    float duration_;
    float elapsed_ = 0.0f;
    bool initialized_ = false;
};

class ResizeTo : public Animation {
  public:
    // Grid-based resizing
    ResizeTo(std::shared_ptr<Animatable> animatable, float gridW, float gridH, float duration = default_duration);
    
    bool update(float dt) override;

  private:
    std::shared_ptr<Animatable> animatable_;
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
    StrokeTo(std::shared_ptr<Animatable> animatable, float targetStroke, float duration);

    bool update(float dt) override;

  private:
    std::shared_ptr<Animatable> animatable_;
    float startStroke_;
    float targetStroke_;
    float duration_;
    float elapsed_ = 0.0f;
};

class ClearWaypoints : public Animation {
  public:
    ClearWaypoints(std::shared_ptr<Line> line, float duration = default_duration);
    bool update(float dt) override;

  private:
    std::shared_ptr<Line> line_;
    std::vector<GridCoord> originalWaypoints_;
    float duration_;
    float elapsed_ = 0.0f;
    bool initialized_ = false;
};

class AddWaypoint : public Animation {
  public:
    AddWaypoint(std::shared_ptr<Line> line, const GridCoord& newWaypoint, float duration = default_duration);
    bool update(float dt) override;

  private:
    std::shared_ptr<Line> line_;
    GridCoord startPos_;
    GridCoord targetWaypoint_;
    float duration_;
    float elapsed_ = 0.0f;
    bool initialized_ = false;
    int waypointIndex_ = -1;
};

class RemoveWaypoint : public Animation {
  public:
    RemoveWaypoint(std::shared_ptr<Line> line, int waypointIndex, float duration = default_duration);
    bool update(float dt) override;

  private:
    std::shared_ptr<Line> line_;
    GridCoord originalWaypoint_;
    GridCoord targetPos_;
    int waypointIndex_;
    float duration_;
    float elapsed_ = 0.0f;
    bool initialized_ = false;
};

class MoveWaypoint : public Animation {
  public:
    MoveWaypoint(std::shared_ptr<Line> line, int waypointIndex, const GridCoord& targetPos, float duration = default_duration);
    
    bool update(float dt) override;

  private:
    std::shared_ptr<Line> line_;
    int waypointIndex_;
    GridCoord startPos_;
    GridCoord targetPos_;
    float duration_;
    float elapsed_ = 0.0f;
    bool initialized_ = false;
};

class MoveLineEnd : public Animation {
  public:
    MoveLineEnd(std::shared_ptr<Line> line, const GridCoord& targetEndPos, float duration = default_duration);
    
    bool update(float dt) override;

  private:
    std::shared_ptr<Line> line_;
    GridCoord startEndPos_;
    GridCoord targetEndPos_;
    float duration_;
    float elapsed_ = 0.0f;
    bool initialized_ = false;
};

class MoveLineStart : public Animation {
  public:
    MoveLineStart(std::shared_ptr<Line> line, const GridCoord& targetStartPos, float duration = default_duration);
    
    bool update(float dt) override;

  private:
    std::shared_ptr<Line> line_;
    GridCoord startStartPos_;
    GridCoord targetStartPos_;
    float duration_;
    float elapsed_ = 0.0f;
    bool initialized_ = false;
};

// Animation group for parallel execution
class AnimationGroup : public Animation {
  public:
    AnimationGroup() = default;
    
    // Add animations to the group
    void add(std::shared_ptr<Animation> animation);
    void addAll(const std::vector<std::shared_ptr<Animation>>& animations);
    
    bool update(float dt) override;
    
    // Check if the group is empty
    bool empty() const { return animations_.empty(); }
    size_t size() const { return animations_.size(); }

  private:
    std::vector<std::shared_ptr<Animation>> animations_;
};

} // namespace banim