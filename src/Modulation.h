#pragma once

namespace sm {

enum class Waveform {
    Sine = 0,
    Triangle,
    Saw,
    Square,
    SmoothSquare,
    Noise
};

struct ModulationParams {
    float frequency = 12.0f;
    float phaseDegrees = 0.0f;
    float amplitude = 0.5f;
    float amount = 1.0f;
};

float waveformValue(Waveform waveform, float phaseNormalized);
float modulate(float signal, float carrier, const ModulationParams& params);

} // namespace sm
