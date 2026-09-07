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
        const auto* src = reinterpret_cast<const PF_Pixel8*>(
            srcBase + static_cast<std::size_t>(y) * srcStride);
        auto* dst = reinterpret_cast<PF_Pixel8*>(
            dstBase + static_cast<std::size_t>(y) * dstStride);
        for (int x = 0; x < width; ++x) {
            const int coordinate = vertical ? y : x;
            const int extent = vertical ? std::max(rows, 1) : std::max(width, 1);
            const float normalized = static_cast<float>(coordinate) / static_cast<float>(extent);
            const float direction = reverse ? -1.0f : 1.0f;
            const float wave = std::sin(normalized * frequency * 6.2831853f * direction + phase);
            const float shaped = std::tanh(wave * (1.0f + distortion * 8.0f));
            const float gain = 1.0f + (invert ? -1.0f : 1.0f) * distortion * shaped;
            const int sampledX = (x / downsample) * downsample;
            const PF_Pixel8& pixel = src[std::min(sampledX, width - 1)];
            PF_Pixel8& result = dst[x];
            result.alpha = pixel.alpha;
            const float factor = 1.0f + (gain - 1.0f) * opacity;
            const float luminance = (pixel.red * 0.299f + pixel.green * 0.587f + pixel.blue * 0.114f) * factor;
            result.red = clampByte(rgb && params[SM_CH1]->u.bd.value ? pixel.red * factor : luminance);
            result.green = clampByte(rgb && params[SM_CH2]->u.bd.value ? pixel.green * factor : luminance);
            result.blue = clampByte(rgb && params[SM_CH3]->u.bd.value ? pixel.blue * factor : luminance);
            if (!ignoreAlpha) {
                result.alpha = clampByte(pixel.alpha * factor);
            }
        }
    }

    return PF_Err_NONE;
}
