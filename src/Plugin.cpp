#include "Plugin.h"

#include <cstdio>

#include "Param_Utils.h"

#include "Params.h"
#include "Render.h"

PF_Err About(PF_InData*, PF_OutData* out_data, PF_ParamDef*[], PF_LayerDef*) {
    std::snprintf(
        out_data->return_msg,
        PF_MAX_EFFECT_MSG_LEN + 1,
        "Signal Modulator v%d.%d\\rOriginal modulation effect (work in progress).",
        static_cast<int>(SM_VERSION_MAJOR),
        static_cast<int>(SM_VERSION_MINOR));
    return PF_Err_NONE;
}

PF_Err GlobalSetup(PF_InData*, PF_OutData* out_data, PF_ParamDef*[], PF_LayerDef*) {
    out_data->my_version = PF_VERSION(
        SM_VERSION_MAJOR,
        SM_VERSION_MINOR,
        SM_VERSION_BUG,
        SM_VERSION_STAGE,
        SM_VERSION_BUILD);

    out_data->out_flags = PF_OutFlag_PIX_INDEPENDENT | PF_OutFlag_DEEP_COLOR_AWARE;
    out_data->out_flags2 = PF_OutFlag2_FLOAT_COLOR_AWARE | PF_OutFlag2_SUPPORTS_THREADED_RENDERING;

    return PF_Err_NONE;
}

PF_Err ParamsSetup(PF_InData* in_data, PF_OutData* out_data, PF_ParamDef*[], PF_LayerDef*) {
    return AddParams(in_data, out_data);
}

PF_Err Render(PF_InData* in_data, PF_OutData* out_data, PF_ParamDef* params[], PF_LayerDef* output) {
    return RenderFrame(in_data, out_data, params, output);
}

extern "C" DllExport PF_Err EffectMain(
    PF_Cmd cmd,
    PF_InData* in_data,
    PF_OutData* out_data,
    PF_ParamDef* params[],
    PF_LayerDef* output,
    void* extra) {
    PF_Err err = PF_Err_NONE;

    switch (cmd) {
    case PF_Cmd_ABOUT:
        err = About(in_data, out_data, params, output);
        break;
    case PF_Cmd_GLOBAL_SETUP:
        err = GlobalSetup(in_data, out_data, params, output);
        break;
    case PF_Cmd_PARAMS_SETUP:
        err = ParamsSetup(in_data, out_data, params, output);
        break;
    case PF_Cmd_RENDER:
        err = Render(in_data, out_data, params, output);
        break;
    default:
        break;
    }

    (void)extra;
    return err;
}
