#pragma once

#include "AE_Effect.h"

constexpr A_u_long SM_VERSION_MAJOR = 0;
constexpr A_u_long SM_VERSION_MINOR = 1;
constexpr A_u_long SM_VERSION_BUG = 0;
constexpr A_u_long SM_VERSION_STAGE = PF_Stage_DEVELOP;
constexpr A_u_long SM_VERSION_BUILD = 1;

enum ParamIndex {
    SM_INPUT = 0,
    SM_MODE,
    SM_FREQUENCY,
    SM_PHASE,
    SM_AMPLITUDE,
    SM_NUM_PARAMS
};

enum ModeValue {
    MODE_MODULATE = 1,
    MODE_DEMODULATE,
    MODE_MODULATE_DEMODULATE
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
