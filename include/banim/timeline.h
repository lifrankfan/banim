#pragma once

#include "banim/animations.h"
#include <vector>
#include <memory>
#include <functional>

namespace banim {

// Advanced timeline for complex computer architecture animations
class Timeline {
public:
    struct KeyFrame {
        float time;
        std::shared_ptr<Animation> animation;
        std::function<void()> callback; // For synchronization points
    };
    
    void addKeyFrame(float time, std::shared_ptr<Animation> animation, std::function<void()> callback = nullptr);
    void addParallelAnimation(std::shared_ptr<Animation> animation); // Run in parallel with current
    void addSequentialAnimation(std::shared_ptr<Animation> animation); // Run after current
    
    bool update(float dt);
    void reset();
    void pause();
    void resume();
    void setSpeed(float speed);
    
    float getCurrentTime() const { return currentTime_; }
    float getTotalDuration() const;
    bool isComplete() const;
    
private:
    std::vector<KeyFrame> keyFrames_;
    std::vector<std::shared_ptr<Animation>> activeAnimations_;
    float currentTime_ = 0.0f;
    float speed_ = 1.0f;
    bool paused_ = false;
    size_t currentKeyFrame_ = 0;
    
    void processKeyFrames();
};

// Animation sequencing helpers for computer architecture demos
class AnimationSequence {
public:
    static std::shared_ptr<Timeline> createDataflowAnimation(
        const std::vector<std::shared_ptr<Shape>>& components,
        const std::vector<std::shared_ptr<Shape>>& dataPath
    );
    
    static std::shared_ptr<Timeline> createPipelineAnimation(
        const std::vector<std::shared_ptr<Shape>>& stages,
        const std::vector<std::shared_ptr<Shape>>& instructions
    );
    
    static std::shared_ptr<Timeline> createMemoryAccessAnimation(
        std::shared_ptr<Shape> cpu,
        std::shared_ptr<Shape> memory,
        std::shared_ptr<Shape> data
    );
};

} // namespace banim
