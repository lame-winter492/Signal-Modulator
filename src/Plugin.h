#pragma once

#include "AE_Effect.h"
#include "entry.h"

constexpr A_u_long SM_VERSION_MAJOR = 0;
constexpr A_u_long SM_VERSION_MINOR = 1;
constexpr A_u_long SM_VERSION_BUG = 0;
constexpr A_u_long SM_VERSION_STAGE = PF_Stage_DEVELOP;
constexpr A_u_long SM_VERSION_BUILD = 1;

enum ParamIndex {
    SM_INPUT = 0,
    SM_OMEGA,
    SM_PHASE,
    SM_DISTORTION,
    SM_LOWPASS_1,
    SM_LOWPASS_2,
    SM_LOWPASS_3,
    SM_LOWPASS_4,
    SM_DOWNSAMPLE,
    SM_DIRECTION,
    SM_ORIENTATION,
    SM_INVERT,
    SM_CONTOUR_MODE,
    SM_NUM_PARAMS
};

PF_Err About(PF_InData* in_data, PF_OutData* out_data, PF_ParamDef* params[], PF_LayerDef* output);
PF_Err GlobalSetup(PF_InData* in_data, PF_OutData* out_data, PF_ParamDef* params[], PF_LayerDef* output);
PF_Err ParamsSetup(PF_InData* in_data, PF_OutData* out_data, PF_ParamDef* params[], PF_LayerDef* output);
PF_Err Render(PF_InData* in_data, PF_OutData* out_data, PF_ParamDef* params[], PF_LayerDef* output);

extern "C" DllExport PF_Err EffectMain(
    PF_Cmd cmd,
    PF_InData* in_data,
    PF_OutData* out_data,
    PF_ParamDef* params[],
    PF_LayerDef* output,
    void* extra);
