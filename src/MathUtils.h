#pragma once

#include <algorithm>
#include <cmath>

namespace sm {

constexpr float kPi = 3.14159265358979323846f;

inline float clamp(float v, float lo, float hi) {
    return std::min(std::max(v, lo), hi);
}

inline float wrap01(float x) {
    float w = std::fmod(x, 1.0f);
    return (w < 0.0f) ? (w + 1.0f) : w;
}

inline float phaseDegreesToNormalized(float degrees) {
    return degrees / 360.0f;
}

} // namespace sm
