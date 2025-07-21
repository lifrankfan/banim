#pragma once

namespace banim {

enum class EasingType {
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    Bounce,
    Elastic,
    Back
};

class Easing {
public:
    static float apply(float t, EasingType type = EasingType::EaseInOut);
    
private:
    static float easeIn(float t);
    static float easeOut(float t);
    static float easeInOut(float t);
    static float bounce(float t);
    static float elastic(float t);
    static float back(float t);
};

} // namespace banim
