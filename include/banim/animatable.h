#pragma once

#include <cairo/cairo.h>
#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include "banim/grid.h"

namespace banim {

class Animation;
class Scene;

class Animatable : public std::enable_shared_from_this<Animatable> {
public:
    virtual ~Animatable() = default;
    virtual void draw(cairo_t* cr) = 0;
    
    // Grid positioning
    virtual float gridX() const { return gridPos_.x; }
    virtual float gridY() const { return gridPos_.y; }
    virtual GridCoord getGridPos() const { return gridPos_; }
    virtual void setGridPos(const GridCoord& pos) { gridPos_ = pos; }
    virtual void setGridPos(float x, float y) { gridPos_ = {x, y}; }
    
    // Grid sizing
    virtual float getGridWidth() const { return gridSize_.x; }
    virtual float getGridHeight() const { return gridSize_.y; }
    virtual void setGridSize(float w, float h) { gridSize_ = {w, h}; }
    virtual void setGridSize(const GridCoord& size) { gridSize_ = size; }
    
    // Visibility and appearance
    virtual void setAlpha(float alpha) { a_ = alpha; }
    virtual float getAlpha() const { return a_; }
    virtual void hide() { setAlpha(0.0f); }
    virtual void show() { setAlpha(1.0f); }
    
    virtual Animatable& setColor(float r, float g, float b, float a = 1.0f) {
        r_ = r; g_ = g; b_ = b; a_ = a;
        return *this;
    }
    
    virtual Animatable& setRotation(float angle) {
        rotation_ = angle;
        return *this;
    }
    
    virtual Animatable& setFilled(bool filled) {
        filled_ = filled;
        return *this;
    }
    
    virtual bool isFilled() const { return filled_; }
    
    virtual Animatable& setStrokeWidth(float w) {
        strokeWidth_ = w;
        return *this;
    }
    
    virtual float getStrokeWidth() const { return strokeWidth_; }
    
    // Animation interface methods
    virtual void getAnimatableSize(float& w, float& h) const = 0;
    virtual void setAnimatableSize(float w, float h) = 0;
    virtual void resetForAnimation() = 0;

protected:
    GridCoord gridPos_{0, 0};
    GridCoord gridSize_{1, 1};
    float r_ = 0, g_ = 0, b_ = 0, a_ = 1;
    float rotation_ = 0;
    float strokeWidth_ = 2.0f;
    bool filled_ = true;
};

class Rectangle : public Animatable {
public:
    Rectangle(const GridCoord& gridPos, float gridWidth, float gridHeight,
              float duration = 0.5f,
              float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f,
              float rotation = 0.0f);
    
    Rectangle& setBorderRadius(float radius);
    float getBorderRadius() const { return borderRadius_; }
    
    void draw(cairo_t* cr) override;
    
    void getAnimatableSize(float& w, float& h) const override {
        w = gridSize_.x;
        h = gridSize_.y;
    }
    
    void setAnimatableSize(float w, float h) override {
        setGridSize(w, h);
    }
    
    void resetForAnimation() override {
        // Nothing special needed for rectangles
    }

private:
    float borderRadius_ = 0.0f;
    float duration_;
};

class Circle : public Animatable {
public:
    Circle(const GridCoord& gridPos, float gridRx, float gridRy,
           float duration = 0.5f,
           float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f,
           float rotation = 0.0f);
    
    void draw(cairo_t* cr) override;
    
    void getAnimatableSize(float& w, float& h) const override {
        w = gridSize_.x;
        h = gridSize_.y;
    }
    
    void setAnimatableSize(float w, float h) override {
        setGridSize(w, h);
    }
    
    void resetForAnimation() override {
        // Nothing special needed for circles
    }

private:
    float duration_;
};

class Line : public Animatable {
public:
    Line(const GridCoord& startPos, const GridCoord& endPos,
         float duration = 0.5f,
         float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
    
    void setEndPos(const GridCoord& endPos) { endPos_ = endPos; }
    GridCoord getEndPos() const { return endPos_; }
    
    // Waypoint management
    void addWaypoint(const GridCoord& waypoint);
    void setWaypoint(int index, const GridCoord& waypoint);
    GridCoord getWaypoint(int index) const;
    int getWaypointCount() const { return waypoints_.size(); }
    void clearWaypoints();
    void removeWaypoint(int index);
    
    // Move entire line (all points)
    void moveBy(float deltaX, float deltaY);
    void moveTo(const GridCoord& newStartPos);
    
    // Override setGridPos to move the entire line
    void setGridPos(const GridCoord& pos) override {
        moveTo(pos);
    }
    void setGridPos(float x, float y) override {
        moveTo({x, y});
    }
    
    void draw(cairo_t* cr) override;
    
    void getAnimatableSize(float& w, float& h) const override {
        // For lines, size represents length and thickness
        float dx = endPos_.x - gridPos_.x;
        float dy = endPos_.y - gridPos_.y;
        w = std::sqrt(dx * dx + dy * dy);
        h = strokeWidth_;
    }
    
    void setAnimatableSize(float w, float h) override {
        // For lines, we can animate the length by adjusting the end position
        float dx = endPos_.x - gridPos_.x;
        float dy = endPos_.y - gridPos_.y;
        float currentLength = std::sqrt(dx * dx + dy * dy);
        
        if (currentLength > 0) {
            float scale = w / currentLength;
            endPos_.x = gridPos_.x + dx * scale;
            endPos_.y = gridPos_.y + dy * scale;
            
            // Scale waypoints too
            for (auto& waypoint : waypoints_) {
                waypoint.x = gridPos_.x + (waypoint.x - gridPos_.x) * scale;
                waypoint.y = gridPos_.y + (waypoint.y - gridPos_.y) * scale;
            }
        }
        
        setStrokeWidth(h);
    }
    
    void resetForAnimation() override {
        // Store original positions for animation
        originalEndPos_ = endPos_;
        originalWaypoints_ = waypoints_;
    }

private:
    GridCoord endPos_;
    GridCoord originalEndPos_;
    std::vector<GridCoord> waypoints_;
    std::vector<GridCoord> originalWaypoints_;
    float duration_;
};

class Text : public Animatable {
public:
    Text(const GridCoord& gridPos,
         const std::string& content,
         float fontSize = 24.0f,
         float duration = 0.5f);
    
    void setText(const std::string& content);
    void setFontSize(float size);
    float getFontSize() const { return fontSize_; }
    void draw(cairo_t* cr) override;
    
    void getAnimatableSize(float& w, float& h) const override {
        w = fontSize_;
        h = fontSize_;
    }
    
    void setAnimatableSize(float w, float h) override {
        setFontSize(w);  // Use width as the new font size
    }
    
    void resetForAnimation() override {
        // Store original font size for animation scaling
        originalFontSize_ = fontSize_;
    }

private:
    std::string content_;
    float fontSize_;
    float duration_;
    float originalFontSize_ = 24.0f;
};

} // namespace banim
