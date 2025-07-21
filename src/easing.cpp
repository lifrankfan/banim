#include "banim/easing.h"
#include <cmath>

namespace banim {

float Easing::apply(float t, EasingType type) {
    switch (type) {
        case EasingType::Linear: return t;
        case EasingType::EaseIn: return easeIn(t);
        case EasingType::EaseOut: return easeOut(t);
        case EasingType::EaseInOut: return easeInOut(t);
        case EasingType::Bounce: return bounce(t);
        case EasingType::Elastic: return elastic(t);
        case EasingType::Back: return back(t);
        default: return t;
    }
}

float Easing::easeIn(float t) {
    return t * t;
}

float Easing::easeOut(float t) {
    return 1 - (1 - t) * (1 - t);
}

float Easing::easeInOut(float t) {
    return t < 0.5f ? 2 * t * t : 1 - 2 * (1 - t) * (1 - t);
}

float Easing::bounce(float t) {
    if (t < 1/2.75f) {
        return 7.5625f * t * t;
    } else if (t < 2/2.75f) {
        t -= 1.5f/2.75f;
        return 7.5625f * t * t + 0.75f;
    } else if (t < 2.5f/2.75f) {
        t -= 2.25f/2.75f;
        return 7.5625f * t * t + 0.9375f;
    } else {
        t -= 2.625f/2.75f;
        return 7.5625f * t * t + 0.984375f;
    }
}

float Easing::elastic(float t) {
    if (t == 0) return 0;
    if (t == 1) return 1;
    float p = 0.3f;
    float s = p / 4;
    return std::pow(2, -10 * t) * std::sin((t - s) * (2 * M_PI) / p) + 1;
}

float Easing::back(float t) {
    float c1 = 1.70158f;
    float c3 = c1 + 1;
    return c3 * t * t * t - c1 * t * t;
}

} // namespace banim
