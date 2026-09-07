#include "Modulation.h"

#include <cmath>

#include "MathUtils.h"

namespace sm {

static float hashNoise(float x) {
    float s = std::sin(x * 127.1f) * 43758.5453f;
    return wrap01(s) * 2.0f - 1.0f;
}

float waveformValue(Waveform waveform, float phaseNormalized) {
    const float p = wrap01(phaseNormalized);

    switch (waveform) {
    case Waveform::Sine:
        return std::sin(2.0f * kPi * p);
    case Waveform::Triangle:
        return 1.0f - 4.0f * std::fabs(p - 0.5f);
    case Waveform::Saw:
        return 2.0f * p - 1.0f;
    case Waveform::Square:
        return (p < 0.5f) ? 1.0f : -1.0f;
    case Waveform::SmoothSquare: {
        const float s = std::sin(2.0f * kPi * p);
        return std::tanh(3.0f * s);
    }
    case Waveform::Noise:
        return hashNoise(p);
    default:
        return 0.0f;
    }
}

float modulate(float signal, float carrier, const ModulationParams& params) {
    const float depth = clamp(params.amplitude, 0.0f, 1.0f);
    const float amount = clamp(params.amount, 0.0f, 1.0f);
    const float gain = 1.0f + (carrier * depth);
    const float mixed = (signal * gain) * amount + signal * (1.0f - amount);
    return mixed;
}

} // namespace sm
