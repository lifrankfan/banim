#pragma once

#include "banim/init.h"
#include <functional>

namespace banim {

/// Draw via a Cairo‐drawing lambda and present via OpenGL.
/// Internally uploads the Cairo image surface into a texture and draws a
/// fullscreen quad.
void draw(const std::function<void(cairo_t *)> &fn);

} // namespace banim
