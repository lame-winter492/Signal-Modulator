#include "Params.h"

#include "AE_Macros.h"
#include "Param_Utils.h"

#include "Plugin.h"

PF_Err AddParams(PF_InData* in_data, PF_OutData* out_data) {
    PF_Err err = PF_Err_NONE;
    PF_ParamDef def;

    AEFX_CLR_STRUCT(def);
    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX("Omega", 0, 1000, 0, 1000, 12, PF_Precision_HUNDREDTHS, 0, 0, SM_OMEGA);

    AEFX_CLR_STRUCT(def);
    PF_ADD_ANGLE("Phase", 0, SM_PHASE);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX("Distortion", 0, 100, 0, 100, 12, PF_Precision_HUNDREDTHS, 0, 0, SM_DISTORTION);
    PF_ADD_CHECKBOX("Lowpass 1", "Enable first lowpass", 1, 0, SM_LOWPASS_1);
    PF_ADD_CHECKBOX("Lowpass 2", "Enable second lowpass", 1, 0, SM_LOWPASS_2);
    PF_ADD_CHECKBOX("Lowpass 3", "Enable third lowpass", 1, 0, SM_LOWPASS_3);
    PF_ADD_CHECKBOX("Lowpass 4", "Enable fourth lowpass", 1, 0, SM_LOWPASS_4);
    PF_ADD_FLOAT_SLIDERX("Downsample", 1, 64, 1, 64, 1, PF_Precision_INTEGER, 0, 0, SM_DOWNSAMPLE);
    PF_ADD_POPUP("Direction", 2, 1, "Forward|Reverse", SM_DIRECTION);
    PF_ADD_POPUP("Orientation", 2, 1, "Horizontal|Vertical", SM_ORIENTATION);
    PF_ADD_CHECKBOX("Invert", "Invert signal", 0, 0, SM_INVERT);
    PF_ADD_FLOAT_SLIDERX("Opacity", 0, 100, 0, 100, 25, PF_Precision_HUNDREDTHS, 0, 0, SM_OPACITY);
    PF_ADD_CHECKBOX("Ignore alpha", "Preserve alpha while processing", 1, 0, SM_IGNORE_ALPHA);
    PF_ADD_CHECKBOX("Hide white line", "Suppress white signal line", 1, 0, SM_HIDE_WHITE_LINE);
    PF_ADD_POPUP("Color scheme", 2, 1, "Grayscale|RGB", SM_COLOR_SCHEME);
    PF_ADD_CHECKBOX("CH1", "Enable channel 1", 1, 0, SM_CH1);
    PF_ADD_CHECKBOX("CH2", "Enable channel 2", 1, 0, SM_CH2);
    PF_ADD_CHECKBOX("CH3", "Enable channel 3", 1, 0, SM_CH3);
    PF_ADD_CHECKBOX("CH4", "Enable channel 4", 1, 0, SM_CH4);
    PF_ADD_CHECKBOX("Contour mode", "Quantize the signal into thin contour lines", 0, 0, SM_CONTOUR_MODE);

    out_data->num_params = SM_NUM_PARAMS;
    return err;
}
