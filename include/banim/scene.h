#pragma once

#include <cairo/cairo.h>
#include <memory>
#include <vector>
#include <queue>
#include <variant>
#include <map>
#include <string>

namespace banim {

class Shape;
class Animation;

struct AddAction {
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Animation> spawnAnimation; // nullptr for no animation
    int layer = 0; // Layer for z-ordering
    std::string tag; // Optional tag for grouping/finding shapes
};

using TimelineAction = std::variant<std::shared_ptr<Animation>, AddAction>;

// Layer system for complex scenes
class Layer {
public:
    Layer(int zIndex = 0, bool visible = true) : zIndex_(zIndex), visible_(visible) {}
    
    void addShape(std::shared_ptr<Shape> shape);
    void removeShape(std::shared_ptr<Shape> shape);
    void render(cairo_t* cr);
    
    void setVisible(bool visible) { visible_ = visible; }
    bool isVisible() const { return visible_; }
    void setOpacity(float opacity) { opacity_ = opacity; }
    float getOpacity() const { return opacity_; }
    
    int getZIndex() const { return zIndex_; }
    void setZIndex(int z) { zIndex_ = z; }
    
private:
    std::vector<std::shared_ptr<Shape>> shapes_;
    int zIndex_;
    bool visible_;
    float opacity_ = 1.0f;
};

class Scene {
  public:
    void add(std::shared_ptr<Shape> shape);
    void add(std::shared_ptr<Shape> shape, std::shared_ptr<Animation> animation);
    void add(std::shared_ptr<Shape> shape, std::shared_ptr<Animation> animation, int layer, const std::string& tag = "");
    
    void play(std::shared_ptr<Animation> anim);
    void renderScene(cairo_t *cr);
    void update(float dt);
    void wait(float duration);
    
    // Layer management
    void createLayer(const std::string& name, int zIndex = 0);
    void setLayerVisible(const std::string& name, bool visible);
    void setLayerOpacity(const std::string& name, float opacity);
    
    // Shape management
    void removeShape(std::shared_ptr<Shape> shape);
    void removeShapesByTag(const std::string& tag);
    std::vector<std::shared_ptr<Shape>> findShapesByTag(const std::string& tag);
    
    // Camera/viewport
    void setCamera(float x, float y, float zoom = 1.0f);
    void moveCamera(float dx, float dy);
    void zoomCamera(float factor);

  private:
    std::map<std::string, std::unique_ptr<Layer>> layers_;
    std::vector<std::shared_ptr<Shape>> shapes_; // Keep for backward compatibility
    std::queue<TimelineAction> timeline_;
    std::shared_ptr<Animation> currentAnimation_ = nullptr;
    
    // Camera state
    float cameraX_ = 0.0f, cameraY_ = 0.0f;
    float cameraZoom_ = 1.0f;
    
    void renderLayers(cairo_t* cr);
    Layer* getOrCreateLayer(int zIndex);
};


} // namespace banim