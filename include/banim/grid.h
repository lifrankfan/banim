#pragma once

namespace banim {

// Grid coordinate system for positioning objects
struct GridCoord {
    float x, y;
    GridCoord(float x = 0, float y = 0) : x(x), y(y) {}
};

} // namespace banim
