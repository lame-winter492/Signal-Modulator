#pragma once

namespace sm {

struct FilterParams {
    float amount = 0.0f;
};

float applyFilter(float signal, const FilterParams& params);

} // namespace sm
