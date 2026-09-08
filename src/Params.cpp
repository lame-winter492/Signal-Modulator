#include "Params.h"

#include "AE_Macros.h"
#include "Param_Utils.h"

#include "Plugin.h"

PF_Err AddParams(PF_InData* in_data, PF_OutData* out_data) {
    PF_Err err = PF_Err_NONE;
    PF_ParamDef def;

    AEFX_CLR_STRUCT(def);
    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX("Omega", 0, 10, 0, 10, 0.95, PF_Precision_THOUSANDTHS, 0, 0, SM_OMEGA);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX("Phase", 0, 1, 0, 1, 0.15, PF_Precision_THOUSANDTHS, 0, 0, SM_PHASE);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX("Distortion", 0, 100, 0, 100, 50, PF_Precision_TENTHS, 0, 0, SM_DISTORTION);
    PF_ADD_CHECKBOX("Lowpass 1", "Enable first lowpass", 0, 0, SM_LOWPASS_1);
    PF_ADD_CHECKBOX("Lowpass 2", "Enable second lowpass", 0, 0, SM_LOWPASS_2);
    PF_ADD_CHECKBOX("Lowpass 3", "Enable third lowpass", 1, 0, SM_LOWPASS_3);
    PF_ADD_CHECKBOX("Lowpass 4", "Enable fourth lowpass", 0, 0, SM_LOWPASS_4);
    PF_ADD_POPUP("Direction", 2, 1, "Forward|Reverse", SM_DIRECTION);
    PF_ADD_POPUP("Orientation", 2, 2, "Horizontal|Vertical", SM_ORIENTATION);
    PF_ADD_CHECKBOX("Invert", "Invert signal", 0, 0, SM_INVERT);
    PF_ADD_FLOAT_SLIDERX("Opacity", 0, 255, 0, 255, 255, PF_Precision_TENTHS, 0, 0, SM_OPACITY);
    PF_ADD_CHECKBOX("Ignore alpha", "Preserve alpha while processing", 1, 0, SM_IGNORE_ALPHA);
    PF_ADD_CHECKBOX("Hide white line", "Suppress white signal line", 1, 0, SM_HIDE_WHITE_LINE);

    out_data->num_params = SM_NUM_PARAMS;
    return err;
}
