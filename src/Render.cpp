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
    const bool vertical = params[SM_ORIENTATION]->u.pd.value == 2;
    const bool reverse = params[SM_DIRECTION]->u.pd.value == 2;
    const bool invert = params[SM_INVERT]->u.bd.value != 0;
    const float opacity = static_cast<float>(params[SM_OPACITY]->u.fs_d.value) / 100.0f;
    const bool ignoreAlpha = params[SM_IGNORE_ALPHA]->u.bd.value != 0;
    const bool rgb = params[SM_COLOR_SCHEME]->u.pd.value == 2;

    for (int y = 0; y < rows; ++y) {
        auto* dst = reinterpret_cast<PF_Pixel8*>(
            dstBase + static_cast<std::size_t>(y) * dstStride);
        for (int x = 0; x < width; ++x) {
            dst[x] = PF_Pixel8{0, 0, 0, 255};
        }
    }

    const int lineStep = std::max(1, downsample);
    const float lineAmplitude = std::max(2.0f, lineStep * 2.0f + distortion * 18.0f);
    const float direction = reverse ? -1.0f : 1.0f;
    const bool smooth = params[SM_LOWPASS_1]->u.bd.value ||
        params[SM_LOWPASS_2]->u.bd.value ||
        params[SM_LOWPASS_3]->u.bd.value ||
        params[SM_LOWPASS_4]->u.bd.value;

    for (int sourceY = 0; sourceY < rows; sourceY += lineStep) {
        const auto* src = reinterpret_cast<const PF_Pixel8*>(
            srcBase + static_cast<std::size_t>(sourceY) * srcStride);
        const int baseline = vertical
            ? static_cast<int>((static_cast<float>(sourceY) / std::max(rows, 1)) * (width - 1))
            : sourceY;

        for (int x = 0; x < width; ++x) {
            const int sampledX = std::min((x / downsample) * downsample, width - 1);
            const int neighborX = std::min(sampledX + (smooth ? downsample : 0), width - 1);
            const PF_Pixel8& pixel = src[sampledX];
            const PF_Pixel8& neighbor = src[neighborX];
            float red = pixel.red;
            float green = pixel.green;
            float blue = pixel.blue;
            if (smooth) {
                red = (red + neighbor.red) * 0.5f;
                green = (green + neighbor.green) * 0.5f;
                blue = (blue + neighbor.blue) * 0.5f;
            }

            float signal = (red * 0.299f + green * 0.587f + blue * 0.114f) / 255.0f;
            if (invert) {
                signal = 1.0f - signal;
            }
            const float carrier = std::sin(
                (static_cast<float>(x) / std::max(width, 1)) * frequency * 6.2831853f * direction +
                phase);
            const float shaped = std::tanh((signal - 0.5f) * (2.0f + distortion * 12.0f));
            const int displacement = static_cast<int>(
                (shaped + carrier * distortion * 0.15f) * lineAmplitude);
            const int target = vertical
                ? static_cast<int>((static_cast<float>(x) / std::max(width, 1)) * (rows - 1)) + displacement
                : baseline + displacement;
            const int targetX = vertical ? baseline : x;
            if (target < 0 || target >= rows || targetX < 0 || targetX >= width) {
                continue;
            }

            const std::uint8_t intensity = clampByte((0.35f + signal * 0.65f) * opacity * 255.0f);
            PF_Pixel8& result = reinterpret_cast<PF_Pixel8*>(
                dstBase + static_cast<std::size_t>(target) * dstStride)[targetX];
            result.alpha = ignoreAlpha ? 255 : pixel.alpha;
            result.red = rgb && params[SM_CH1]->u.bd.value ? intensity : intensity;
            result.green = rgb && params[SM_CH2]->u.bd.value ? intensity : intensity;
            result.blue = rgb && params[SM_CH3]->u.bd.value ? intensity : intensity;
            if (!params[SM_HIDE_WHITE_LINE]->u.bd.value && intensity > 220) {
                result.red = result.green = result.blue = 255;
            }
        }
    }

    return PF_Err_NONE;
}
