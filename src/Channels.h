#pragma once

namespace sm {

struct RGB {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 1.0f;
};

RGB processChannels(const RGB& in);

} // namespace sm
