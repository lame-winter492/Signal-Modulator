#pragma once

namespace sm {

struct DistortionParams {
    float amount = 0.0f;
};

float applyDistortion(float signal, const DistortionParams& params);

} // namespace sm
