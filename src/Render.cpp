#include "Render.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "Plugin.h"

namespace {

std::uint8_t clampByte(float value) {
    return static_cast<std::uint8_t>(std::clamp(value, 0.0f, 255.0f));
}

}

PF_Err RenderFrame(PF_InData*, PF_OutData*, PF_ParamDef* params[], PF_LayerDef* output) {
    if (!params || !params[SM_INPUT] || !output || !output->data) {
        return PF_Err_BAD_CALLBACK_PARAM;
    }

    const PF_LayerDef* input = &params[SM_INPUT]->u.ld;
    if (!input->data || input->width <= 0 || input->height <= 0) {
        return PF_Err_NONE;
    }

    const auto* srcBase = reinterpret_cast<const std::uint8_t*>(input->data);
    auto* dstBase = reinterpret_cast<std::uint8_t*>(output->data);

    const int rows = std::min(input->height, output->height);
    const int srcStride = std::abs(input->rowbytes);
    const int dstStride = std::abs(output->rowbytes);
    const int width = std::min(input->width, output->width);
    const float frequency = static_cast<float>(params[SM_OMEGA]->u.fs_d.value);
    const float phase = static_cast<float>(params[SM_PHASE]->u.ad.value) / 65536.0f;
    const float distortion = static_cast<float>(params[SM_DISTORTION]->u.fs_d.value) / 100.0f;
    const int downsample = std::max(1, static_cast<int>(params[SM_DOWNSAMPLE]->u.fs_d.value));
    // The host control describes the signal orientation, while the rasterizer
    // walks the perpendicular axis. Keep the UI semantics aligned with the
    // reference workflow: Vertical produces horizontal traces and vice versa.
    const bool vertical = params[SM_ORIENTATION]->u.pd.value == 1;
    const bool reverse = params[SM_DIRECTION]->u.pd.value == 2;
    const bool invert = params[SM_INVERT]->u.bd.value != 0;
    const float opacity = static_cast<float>(params[SM_OPACITY]->u.fs_d.value) / 100.0f;
    const bool ignoreAlpha = params[SM_IGNORE_ALPHA]->u.bd.value != 0;
    const bool rgb = params[SM_COLOR_SCHEME]->u.pd.value == 2;
    const bool contourMode = params[SM_CONTOUR_MODE]->u.bd.value != 0;

    for (int y = 0; y < rows; ++y) {
        auto* dst = reinterpret_cast<PF_Pixel8*>(
            dstBase + static_cast<std::size_t>(y) * dstStride);
        for (int x = 0; x < width; ++x) {
            dst[x] = PF_Pixel8{255, 0, 0, 0};
        }
    }

    // Each source row becomes one thin scanline. Downsample affects the sampled
    // signal, not the spacing of the output lines.
    const int lowpassStages =
        (params[SM_LOWPASS_1]->u.bd.value ? 1 : 0) +
        (params[SM_LOWPASS_2]->u.bd.value ? 1 : 0) +
        (params[SM_LOWPASS_3]->u.bd.value ? 1 : 0) +
        (params[SM_LOWPASS_4]->u.bd.value ? 1 : 0);
    const int lineStep = contourMode ? std::max(2, 5 - lowpassStages) : 1;
    const float lineAmplitude = contourMode
        ? std::max(4.0f, 2.0f + distortion * 36.0f)
        : std::max(4.0f, 4.0f + distortion * 80.0f);
    const float direction = reverse ? -1.0f : 1.0f;

    // The rasterizer reads along the axis perpendicular to the displayed
    // traces, matching the orientation names used by the reference effect.
    const int lineCount = vertical ? width : rows;
    const int sampleLength = vertical ? rows : width;
    for (int line = 0; line < lineCount; line += lineStep) {
        const int baseline = line;
        int previousTarget = baseline;
        bool previousContour = false;
        for (int position = 0; position < sampleLength; ++position) {
            const int sampledPosition = std::min(
                (position / downsample) * downsample, sampleLength - 1);
            const int radius = lowpassStages * std::max(1, downsample);
            float red = 0.0f;
            float green = 0.0f;
            float blue = 0.0f;
            float alpha = 0.0f;
            int samples = 0;
            for (int offset = -radius; offset <= radius; offset += std::max(1, downsample)) {
                const int filteredPosition = std::clamp(
                    sampledPosition + offset, 0, sampleLength - 1);
                const int filteredX = vertical ? line : filteredPosition;
                const int filteredY = vertical ? filteredPosition : line;
                const auto* filteredRow = reinterpret_cast<const PF_Pixel8*>(
                    srcBase + static_cast<std::size_t>(filteredY) * srcStride);
                const PF_Pixel8& sample = filteredRow[filteredX];
                red += sample.red;
                green += sample.green;
                blue += sample.blue;
                alpha += sample.alpha;
                ++samples;
            }
            red /= static_cast<float>(samples);
            green /= static_cast<float>(samples);
            blue /= static_cast<float>(samples);
            alpha /= static_cast<float>(samples);
            const float luminance = (red * 0.299f + green * 0.587f + blue * 0.114f) / 255.0f;

            const float rgbChannelCount =
                (params[SM_CH1]->u.bd.value ? 1.0f : 0.0f) +
                (params[SM_CH2]->u.bd.value ? 1.0f : 0.0f) +
                (params[SM_CH3]->u.bd.value ? 1.0f : 0.0f);
            float signal = rgbChannelCount > 0.0f
                ? ((params[SM_CH1]->u.bd.value ? red : 0.0f) +
                   (params[SM_CH2]->u.bd.value ? green : 0.0f) +
                   (params[SM_CH3]->u.bd.value ? blue : 0.0f)) /
                      (255.0f * rgbChannelCount)
                : luminance;
            const float alphaMask = params[SM_CH4]->u.bd.value
                ? alpha / 255.0f
                : 1.0f;
            if (invert) {
                signal = 1.0f - signal;
            }
            const int previousPosition = std::max(
                0, sampledPosition - std::max(1, downsample));
            const int previousX = vertical ? line : previousPosition;
            const int previousY = vertical ? previousPosition : line;
            const auto* previousRow = reinterpret_cast<const PF_Pixel8*>(
                srcBase + static_cast<std::size_t>(previousY) * srcStride);
            const PF_Pixel8& previousPixel = previousRow[previousX];
            const float previousLuminance =
                (previousPixel.red * 0.299f +
                 previousPixel.green * 0.587f +
                 previousPixel.blue * 0.114f) / 255.0f;
            const float edge = std::abs(luminance - previousLuminance);
            const float carrier = std::sin(
                (static_cast<float>(position) / std::max(sampleLength, 1)) *
                    frequency * 6.2831853f * direction +
                phase);
            const float centered = signal - 0.5f;
            const float shaped = std::tanh(centered * (2.0f + distortion * 6.0f));
            const int displacement = static_cast<int>(
                (shaped + carrier * distortion * 0.02f) * lineAmplitude);
            // The waveform is displaced perpendicular to its read direction.
            const int targetX = vertical ? baseline + displacement : position;
            const int targetY = vertical ? position : baseline + displacement;
            if (targetY < 0 || targetY >= rows || targetX < 0 || targetX >= width) {
                previousContour = false;
                continue;
            }

            const float displaySignal = std::pow(std::clamp(signal, 0.0f, 1.0f), 1.35f);
            const float contourBands = 3.0f + distortion * 10.0f;
            const float contourPhase = displaySignal * contourBands;
            const float contourDistance = std::abs(
                contourPhase - std::round(contourPhase));
            const bool contourLine = contourMode
                ? displaySignal > std::max(0.10f, 0.25f - distortion * 0.12f)
                : edge > (0.035f + (1.0f - distortion) * 0.08f) ||
                  (contourDistance < 0.018f && displaySignal > 0.18f);
            const std::uint8_t intensity = contourMode
                ? (contourLine ? clampByte(opacity * alphaMask * 255.0f) : 0)
                : clampByte(displaySignal * opacity * alphaMask * 255.0f);
            PF_Pixel8& result = reinterpret_cast<PF_Pixel8*>(
                dstBase + static_cast<std::size_t>(targetY) * dstStride)[targetX];
            result.alpha = ignoreAlpha ? 255 : clampByte(alpha * alphaMask);
            if (rgb && !contourMode) {
                result.red = params[SM_CH1]->u.bd.value ? clampByte(red * opacity) : 0;
                result.green = params[SM_CH2]->u.bd.value ? clampByte(green * opacity) : 0;
                result.blue = params[SM_CH3]->u.bd.value ? clampByte(blue * opacity) : 0;
            } else {
                result.red = result.green = result.blue = intensity;
            }
            if (!params[SM_HIDE_WHITE_LINE]->u.bd.value && intensity > 220) {
                result.red = result.green = result.blue = 255;
            }

            if (contourMode && contourLine && previousContour && targetX > 0) {
                const int bridgeStart = std::min(previousTarget, targetY);
                const int bridgeEnd = std::max(previousTarget, targetY);
                for (int bridgeY = bridgeStart; bridgeY <= bridgeEnd; ++bridgeY) {
                    auto* bridgeRow = reinterpret_cast<PF_Pixel8*>(
                        dstBase + static_cast<std::size_t>(bridgeY) * dstStride);
                    bridgeRow[targetX - 1].red = 255;
                    bridgeRow[targetX - 1].green = 255;
                    bridgeRow[targetX - 1].blue = 255;
                    bridgeRow[targetX - 1].alpha = ignoreAlpha ? 255 : 255;
                }
            }
            previousTarget = targetY;
            previousContour = contourMode && contourLine;
        }
    }

    return PF_Err_NONE;
}
