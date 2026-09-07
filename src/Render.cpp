#include "Render.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>

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
    const float frequency = static_cast<float>(params[SM_FREQUENCY]->u.fs_d.value);
    const float phase = static_cast<float>(params[SM_PHASE]->u.ad.value) / 65536.0f;
    const float amplitude = static_cast<float>(params[SM_AMPLITUDE]->u.fs_d.value) / 100.0f;
    const int mode = params[SM_MODE]->u.pd.value;

    for (int y = 0; y < rows; ++y) {
        const auto* src = reinterpret_cast<const PF_Pixel8*>(
            srcBase + static_cast<std::size_t>(y) * srcStride);
        auto* dst = reinterpret_cast<PF_Pixel8*>(
            dstBase + static_cast<std::size_t>(y) * dstStride);
        for (int x = 0; x < width; ++x) {
            const float carrier = std::sin(
                (static_cast<float>(x) / static_cast<float>(std::max(width, 1))) *
                    frequency * 6.2831853f +
                phase);
            const float gain = 1.0f + amplitude * carrier;
            const PF_Pixel8& pixel = src[x];
            PF_Pixel8& result = dst[x];
            result.alpha = pixel.alpha;
            if (mode == MODE_DEMODULATE) {
                const float inverse = 1.0f / std::max(gain, 0.05f);
                result.red = clampByte(pixel.red * inverse);
                result.green = clampByte(pixel.green * inverse);
                result.blue = clampByte(pixel.blue * inverse);
            } else {
                result.red = clampByte(pixel.red * gain);
                result.green = clampByte(pixel.green * gain);
                result.blue = clampByte(pixel.blue * gain);
            }
        }
    }

    return PF_Err_NONE;
}
