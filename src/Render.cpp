#include "Render.h"

#include <algorithm>
#include <cstdint>
#include <cstring>

#include "Plugin.h"

PF_Err RenderFrame(PF_InData*, PF_OutData*, PF_ParamDef* params[], PF_LayerDef* output) {
    if (!params || !params[SM_INPUT] || !output || !output->data) {
        return PF_Err_BAD_CALLBACK_PARAM;
    }

    const PF_LayerDef* input = &params[SM_INPUT]->u.ld;
    if (!input->data || input->width <= 0 || input->height <= 0) {
        return PF_Err_NONE;
    }

    const auto* srcBase = static_cast<const std::uint8_t*>(input->data);
    auto* dstBase = static_cast<std::uint8_t*>(output->data);

    const int rows = std::min(input->height, output->height);
    const int srcStride = std::abs(input->rowbytes);
    const int dstStride = std::abs(output->rowbytes);
    const int rowCopySize = std::min(srcStride, dstStride);

    for (int y = 0; y < rows; ++y) {
        const std::uint8_t* src = srcBase + static_cast<std::size_t>(y) * srcStride;
        std::uint8_t* dst = dstBase + static_cast<std::size_t>(y) * dstStride;
        std::memcpy(dst, src, static_cast<std::size_t>(rowCopySize));
    }

    return PF_Err_NONE;
}
