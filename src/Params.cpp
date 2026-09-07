#include "Params.h"

#include "AE_Macros.h"
#include "Param_Utils.h"

#include "Plugin.h"

PF_Err AddParams(PF_InData* in_data, PF_OutData* out_data) {
    PF_Err err = PF_Err_NONE;
    PF_ParamDef def;

    AEFX_CLR_STRUCT(def);
    PF_ADD_POPUP("Mode", 3, 1, "Modulate|Demodulate|Modulate + Demodulate", SM_MODE);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Frequency",
        0,
        1000,
        0,
        100,
        12,
        PF_Precision_HUNDREDTHS,
        0,
        0,
        SM_FREQUENCY);

    AEFX_CLR_STRUCT(def);
    PF_ADD_ANGLE("Phase", 0, SM_PHASE);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Amplitude",
        0,
        100,
        0,
        100,
        50,
        PF_Precision_HUNDREDTHS,
        0,
        0,
        SM_AMPLITUDE);

    out_data->num_params = SM_NUM_PARAMS;
    return err;
}
