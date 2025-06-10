/*
 * Copyright (C) 2023 STABL Energy GmbH
 */

/**
 * @defgroup    drivers_battery_LG_E60_8s
 * @ingroup     drivers_battery
 * @{
 *
 * @file        battery_info.h
 * @brief       LG_E60_8S
 *
 * @author      Ao Yang (ao.yang@stabl.com)
 */
#ifndef LG_E60_8S_BATTERY_INFO_H
#define LG_E60_8S_BATTERY_INFO_H

#include "core_defines.h"
#ifdef CONFIG_BMS_EOL_TEST
#include "driver/battery/adbms1818_eol_testbench/eol_static_config.h"
#endif

/**
 * Battery System Information
 * see https://stabl.atlassian.net/wiki/spaces/Batt/pages/1727102978/LG+Chem+E60+8s3p
 */

#define NUM_CELLS                                16U                       /**< Battery module configuration: number of cells in series */
#define NUM_PARALLEL_CELLS                       3U                        /**< Battery module configuration: number of cells in parallel */
#define NUM_PARALLEL_BAT                         1U                        /**< Number of parallel batteries per STABL Module */
static const float maxOperateChargeCurr          = -100.0f;                 /**< Maximum constant charging current @ 25degree & 50% SOC, [A] */
static const float maxOperateDischargeCurr       = 100.0f;                 /**< Maximum constant discharging current @ 25degree & 50% SOC, [A] */

/** Dynamic operating limit curves (for max charge and discharge current) are abled for now. Derating has to be
 * checked and adapted before enabling them as both curves currently overlap. */
#define DYNAMIC_CURRENT_OPL

/** Cell voltage at 25degree, charging direction, for  0, 5, 25, 73, 80, 90, 100% SOC */
static const float maxOperateChargeCurrV[] = {3.261f, 3.425f, 3.517f, 3.952f, 4.023f, 4.123f, 4.216f};
/** Maximum charge current for each cell voltage level (above). */
static const float maxOperateChargeCurrI[] = {-60.f, -60.0f, -100.0f, -100.0f, -90.0f, -60.0f, -60.0f};

/** Cell voltage at 25degree, discharging direction, for 0, 5, 30, 100% SOC */
static const float maxOperateDischargeCurrV[] = {3.261f, 3.425f, 3.620f, 4.216f};
/** Maximum discharge current for each cell voltage level (above). */
static const float maxOperateDischargeCurrI[] = {60.0f, 60.0f, 100.0f, 100.0f};

#if IS_USED(CONFIG_ADBMS1818)
#define VOLTAGE_MEASUREMENT_COMPENSATION

// Offset values for E78 battery voltage cell measurements when measured with ADBMS1818
// Values taken from https://stabl.atlassian.net/wiki/spaces/Hardware/pages/1855029251/CMU+cell+voltage+measurement+offset#Tray-with-two-E60-8s3p-batteries
static const float cellVoltMeasOffsets[NUM_CELLS] = {0.0048, 0, 0.0311, -0.033, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif

/* Temperature Sensor Information */
#define NUM_TEMP_SENSORS                         2U                         /**< Number of temperature sensors */
static const float ntcBetaValue                  = 3435.0f;                 /**< Beta Value at 25/85 degree */
static const float ntcResReference               = 10000.0f;                /**< Resistance value at 25 degree C */
static const float ntcResBridge                  = 10000.0f;               /**< Resistance of the bridge resistor in the battery adapter PCB*/

#define EOL_TEMP_INPUT_REFERENCE_CELSIUS    (24.2f)     /** Expected temperature value in ideal condition  for BMS EOL TEST*/
#define EOL_TEMP_TOTAL_MEASUREMENT_ERROR    (0.6f)      /** Maximum allowed error for temperature measurement in celsius for BMS EOL TEST*/

/* Cell Information */
#ifndef CONFIG_BMS_EOL_TEST
static const float uCellMax                      = 4.216f;                             /**< Upper operating limit for cell voltage at constant current */
static const float uCellMin                      = 3.261f;                             /**< Lower operating limit for cell voltage at constant current */
/** Voltage required in the cell (with the lowest voltage) to start balancing */
/** Find more information here:https://stabl.atlassian.net/wiki/spaces/DEV/pages/1400275031/Cell+Balancing */
static const float uCellMinBalancingVoltage = 3.620f; /**< This voltage should correspond to when SOC= 30% during the discharge */
#endif
static const float uBatModuleMax                  = ((float)NUM_CELLS * uCellMax);      /**< Upper cut-off battery module voltage at constant current operating */
static const float uBatModuleMin                  = ((float)NUM_CELLS * uCellMin);      /**< Lower cut-off battery module voltage at constant current operating */

/* Voltage_based Derating Information */
static const float maxDeratingChargeCurr         = -100.0f;                            /**< Maximum constant charging current for derating [A] */
static const float maxDeratingDischargeCurr      = 100.0f;                             /**< Maximum constant discharging current for derating [A] */
static const float uCellDischargeDeratingZero    = 3.425f;                             /**< [V] lower limit used for derating interpolation, valid during discharge. Voltages below this get rating 0 */
static const float uCellDischargeDeratingOne     = 3.6230f;                             /**< [V] upper limit used for derating interpolation, valid during discharge. Voltages above this get rating 1 */
static const float uCellChargeDeratingOne        = 3.949f;                             /**< [V] lower limit used for derating interpolation, valid during charging. Voltages below this get rating 1 */
static const float uCellChargeDeratingZero       = 4.123f;                             /**< [V] upper limit used for derating interpolation, valid during charging. Voltages above this get rating 0 */

/** @brief Max allowed difference cell voltage difference before cells to start balancing
 *
 * A cell voltage difference (calculated as given cell voltage - voltage of cell with lowest
 * voltage) greater than uCellDiffMax means the particular cell needs to be balanced.
 */
static const float uCellDiffMax       = 0.050f;

/**
 * @brief Cell Hysterisis limits for balancing
 *
 * Once a cell starts balancing it is allowed to discharge till delta is greater than
 * (uCellDiffMax - uCellDiffHysteresis). In this case the balancing continues
 * till delta reached 0.025v ( i.e (0.050f - 0.025f)) for a given cell.
 *
 * @note The value here needs to be approached with caution since a very low
 * hysterisis , say less than 0.015f (depends of battery and OCV), can result
 * in large oscillations in cell voltages which could potentially lead to
 * continuous balancing and hence race conditions.h
*/
static const float uCellDiffHysteresis  = 0.025f;

/** The operating limit is 0°C to 50°C, unless the value from manufacturer is smaller than this range, then we should
 * apply the value from battery manufacturer */
static const float tBatMax = 50.0f; /**< Maximum battery temperature limit */
static const float tBatMin = 0.0f;  /**< Minimum battery temperature limit */

static const float batCap             = 177.0f;         /**< Capacity of used cells in [Ah] */
static const float batR0              = 0.0126f;        /**< The resistance of the battery module, [ohm]*/
static const float OCVcharge[1001]    = {3.261f, 3.267f, 3.272f, 3.278f, 3.283f, 3.288f, 3.294f, 3.299f, 3.303f, 3.308f, 3.313f, 3.317f, 3.322f, 3.326f, 3.33f, 3.335f, 3.339f, 3.343f, 3.346f, 3.35f, 3.354f, 3.357f, 3.361f, 3.364f, 3.367f, 3.37f, 3.374f, 3.377f, 3.379f, 3.382f, 3.385f, 3.388f, 3.39f, 3.393f, 3.395f, 3.398f, 3.4f, 3.402f, 3.404f, 3.406f, 3.408f, 3.41f, 3.412f, 3.414f, 3.416f, 3.417f, 3.419f, 3.421f, 3.422f, 3.424f, 3.425f, 3.426f, 3.428f, 3.429f, 3.43f, 3.431f, 3.433f, 3.434f, 3.435f, 3.436f, 3.437f, 3.438f, 3.439f, 3.439f, 3.44f, 3.441f, 3.442f, 3.443f, 3.443f, 3.444f, 3.445f, 3.445f, 3.446f, 3.447f, 3.447f, 3.448f, 3.449f, 3.449f, 3.45f, 3.45f, 3.451f, 3.451f, 3.452f, 3.453f, 3.453f, 3.454f, 3.454f, 3.455f, 3.455f, 3.456f, 3.456f, 3.457f, 3.458f, 3.458f, 3.459f, 3.46f, 3.46f, 3.461f, 3.462f, 3.462f, 3.463f, 3.464f, 3.465f, 3.465f, 3.466f, 3.467f, 3.468f, 3.469f, 3.47f, 3.471f, 3.472f, 3.473f, 3.474f, 3.475f, 3.476f, 3.477f, 3.478f, 3.479f, 3.48f, 3.481f, 3.482f, 3.483f, 3.484f, 3.485f, 3.487f, 3.488f, 3.489f, 3.49f, 3.491f, 3.492f, 3.494f, 3.495f, 3.496f, 3.497f, 3.498f, 3.499f, 3.501f, 3.502f, 3.503f, 3.504f, 3.505f, 3.507f, 3.508f, 3.509f, 3.51f, 3.511f, 3.512f, 3.514f, 3.515f, 3.516f, 3.517f, 3.518f, 3.519f, 3.52f, 3.521f, 3.522f, 3.524f, 3.525f, 3.526f, 3.527f, 3.528f, 3.529f, 3.53f, 3.531f, 3.532f, 3.533f, 3.534f, 3.535f, 3.536f, 3.537f, 3.538f, 3.539f, 3.539f, 3.54f, 3.541f, 3.542f, 3.543f, 3.544f, 3.545f, 3.546f, 3.547f, 3.548f, 3.548f, 3.549f, 3.55f, 3.551f, 3.552f, 3.553f, 3.553f, 3.554f, 3.555f, 3.556f, 3.557f, 3.558f, 3.558f, 3.559f, 3.56f, 3.561f, 3.561f, 3.562f, 3.563f, 3.564f, 3.565f, 3.565f, 3.566f, 3.567f, 3.568f, 3.568f, 3.569f, 3.57f, 3.57f, 3.571f, 3.572f, 3.573f, 3.573f, 3.574f, 3.575f, 3.575f, 3.576f, 3.577f, 3.577f, 3.578f, 3.579f, 3.579f, 3.58f, 3.581f, 3.581f, 3.582f, 3.583f, 3.583f, 3.584f, 3.585f, 3.585f, 3.586f, 3.587f, 3.587f, 3.588f, 3.588f, 3.589f, 3.59f, 3.59f, 3.591f, 3.591f, 3.592f, 3.593f, 3.593f, 3.594f, 3.594f, 3.595f, 3.595f, 3.596f, 3.597f, 3.597f, 3.598f, 3.598f, 3.599f, 3.599f, 3.6f, 3.6f, 3.601f, 3.601f, 3.602f, 3.602f, 3.603f, 3.603f, 3.604f, 3.604f, 3.605f, 3.605f, 3.606f, 3.606f, 3.607f, 3.607f, 3.608f, 3.608f, 3.609f, 3.609f, 3.61f, 3.61f, 3.611f, 3.611f, 3.612f, 3.612f, 3.613f, 3.613f, 3.614f, 3.614f, 3.614f, 3.615f, 3.615f, 3.616f, 3.616f, 3.617f, 3.617f, 3.617f, 3.618f, 3.618f, 3.619f, 3.619f, 3.62f, 3.62f, 3.62f, 3.621f, 3.621f, 3.622f, 3.622f, 3.622f, 3.623f, 3.623f, 3.624f, 3.624f, 3.624f, 3.625f, 3.625f, 3.626f, 3.626f, 3.626f, 3.627f, 3.627f, 3.628f, 3.628f, 3.628f, 3.629f, 3.629f, 3.629f, 3.63f, 3.63f, 3.631f, 3.631f, 3.631f, 3.632f, 3.632f, 3.632f, 3.633f, 3.633f, 3.634f, 3.634f, 3.634f, 3.635f, 3.635f, 3.635f, 3.636f, 3.636f, 3.636f, 3.637f, 3.637f, 3.638f, 3.638f, 3.638f, 3.639f, 3.639f, 3.639f, 3.64f, 3.64f, 3.64f, 3.641f, 3.641f, 3.641f, 3.642f, 3.642f, 3.642f, 3.643f, 3.643f, 3.643f, 3.644f, 3.644f, 3.644f, 3.645f, 3.645f, 3.645f, 3.646f, 3.646f, 3.646f, 3.647f, 3.647f, 3.648f, 3.648f, 3.648f, 3.649f, 3.649f, 3.649f, 3.65f, 3.65f, 3.65f, 3.651f, 3.651f, 3.651f, 3.652f, 3.652f, 3.652f, 3.653f, 3.653f, 3.653f, 3.654f, 3.654f, 3.654f, 3.655f, 3.655f, 3.655f, 3.656f, 3.656f, 3.656f, 3.657f, 3.657f, 3.657f, 3.658f, 3.658f, 3.658f, 3.659f, 3.659f, 3.659f, 3.66f, 3.66f, 3.661f, 3.661f, 3.661f, 3.662f, 3.662f, 3.662f, 3.663f, 3.663f, 3.663f, 3.664f, 3.664f, 3.665f, 3.665f, 3.665f, 3.666f, 3.666f, 3.667f, 3.667f, 3.667f, 3.668f, 3.668f, 3.669f, 3.669f, 3.669f, 3.67f, 3.67f, 3.671f, 3.671f, 3.672f, 3.672f, 3.672f, 3.673f, 3.673f, 3.674f, 3.674f, 3.675f, 3.675f, 3.676f, 3.676f, 3.677f, 3.677f, 3.678f, 3.678f, 3.679f, 3.679f, 3.68f, 3.68f, 3.681f, 3.681f, 3.682f, 3.683f, 3.683f, 3.684f, 3.684f, 3.685f, 3.686f, 3.686f, 3.687f, 3.687f, 3.688f, 3.689f, 3.689f, 3.69f, 3.691f, 3.691f, 3.692f, 3.693f, 3.693f, 3.694f, 3.695f, 3.695f, 3.696f, 3.697f, 3.698f, 3.698f, 3.699f, 3.7f, 3.701f, 3.701f, 3.702f, 3.703f, 3.704f, 3.705f, 3.706f, 3.706f, 3.707f, 3.708f, 3.709f, 3.71f, 3.711f, 3.712f, 3.713f, 3.714f, 3.714f, 3.715f, 3.716f, 3.717f, 3.718f, 3.719f, 3.72f, 3.721f, 3.722f, 3.723f, 3.724f, 3.725f, 3.726f, 3.727f, 3.728f, 3.73f, 3.731f, 3.732f, 3.733f, 3.734f, 3.735f, 3.736f, 3.737f, 3.738f, 3.739f, 3.741f, 3.742f, 3.743f, 3.744f, 3.745f, 3.746f, 3.747f, 3.749f, 3.75f, 3.751f, 3.752f, 3.753f, 3.754f, 3.756f, 3.757f, 3.758f, 3.759f, 3.76f, 3.762f, 3.763f, 3.764f, 3.765f, 3.766f, 3.768f, 3.769f, 3.77f, 3.771f, 3.772f, 3.774f, 3.775f, 3.776f, 3.777f, 3.779f, 3.78f, 3.781f, 3.782f, 3.783f, 3.785f, 3.786f, 3.787f, 3.788f, 3.79f, 3.791f, 3.792f, 3.793f, 3.795f, 3.796f, 3.797f, 3.798f, 3.799f, 3.801f, 3.802f, 3.803f, 3.804f, 3.805f, 3.807f, 3.808f, 3.809f, 3.81f, 3.811f, 3.813f, 3.814f, 3.815f, 3.816f, 3.817f, 3.818f, 3.82f, 3.821f, 3.822f, 3.823f, 3.824f, 3.825f, 3.826f, 3.828f, 3.829f, 3.83f, 3.831f, 3.832f, 3.833f, 3.834f, 3.835f, 3.836f, 3.837f, 3.838f, 3.84f, 3.841f, 3.842f, 3.843f, 3.844f, 3.845f, 3.846f, 3.847f, 3.848f, 3.849f, 3.85f, 3.851f, 3.852f, 3.853f, 3.854f, 3.855f, 3.856f, 3.857f, 3.858f, 3.859f, 3.86f, 3.861f, 3.862f, 3.863f, 3.864f, 3.865f, 3.866f, 3.867f, 3.868f, 3.869f, 3.87f, 3.871f, 3.872f, 3.873f, 3.874f, 3.875f, 3.876f, 3.877f, 3.878f, 3.879f, 3.88f, 3.881f, 3.882f, 3.883f, 3.884f, 3.884f, 3.885f, 3.886f, 3.887f, 3.888f, 3.889f, 3.89f, 3.891f, 3.892f, 3.893f, 3.894f, 3.895f, 3.896f, 3.897f, 3.898f, 3.899f, 3.899f, 3.9f, 3.901f, 3.902f, 3.903f, 3.904f, 3.905f, 3.906f, 3.907f, 3.908f, 3.909f, 3.91f, 3.911f, 3.912f, 3.913f, 3.914f, 3.914f, 3.915f, 3.916f, 3.917f, 3.918f, 3.919f, 3.92f, 3.921f, 3.922f, 3.923f, 3.924f, 3.925f, 3.926f, 3.927f, 3.928f, 3.929f, 3.93f, 3.931f, 3.932f, 3.933f, 3.933f, 3.934f, 3.935f, 3.936f, 3.937f, 3.938f, 3.939f, 3.94f, 3.941f, 3.942f, 3.943f, 3.944f, 3.945f, 3.946f, 3.947f, 3.948f, 3.949f, 3.95f, 3.951f, 3.952f, 3.953f, 3.954f, 3.955f, 3.956f, 3.957f, 3.958f, 3.959f, 3.96f, 3.961f, 3.962f, 3.963f, 3.964f, 3.965f, 3.966f, 3.967f, 3.968f, 3.969f, 3.97f, 3.971f, 3.972f, 3.973f, 3.974f, 3.975f, 3.976f, 3.977f, 3.978f, 3.979f, 3.98f, 3.981f, 3.982f, 3.983f, 3.984f, 3.985f, 3.986f, 3.987f, 3.988f, 3.989f, 3.99f, 3.991f, 3.992f, 3.994f, 3.995f, 3.996f, 3.997f, 3.998f, 3.999f, 4.0f, 4.001f, 4.002f, 4.003f, 4.004f, 4.005f, 4.006f, 4.007f, 4.008f, 4.009f, 4.01f, 4.011f, 4.012f, 4.013f, 4.014f, 4.016f, 4.017f, 4.018f, 4.019f, 4.02f, 4.021f, 4.022f, 4.023f, 4.024f, 4.025f, 4.026f, 4.027f, 4.028f, 4.029f, 4.031f, 4.032f, 4.033f, 4.034f, 4.035f, 4.036f, 4.037f, 4.038f, 4.039f, 4.04f, 4.041f, 4.042f, 4.043f, 4.044f, 4.046f, 4.047f, 4.048f, 4.049f, 4.05f, 4.051f, 4.052f, 4.053f, 4.054f, 4.055f, 4.056f, 4.057f, 4.058f, 4.059f, 4.061f, 4.062f, 4.063f, 4.064f, 4.065f, 4.066f, 4.067f, 4.068f, 4.069f, 4.07f, 4.071f, 4.072f, 4.073f, 4.074f, 4.075f, 4.076f, 4.077f, 4.078f, 4.079f, 4.08f, 4.081f, 4.082f, 4.083f, 4.084f, 4.085f, 4.086f, 4.087f, 4.088f, 4.089f, 4.09f, 4.091f, 4.092f, 4.093f, 4.094f, 4.095f, 4.096f, 4.097f, 4.098f, 4.099f, 4.1f, 4.101f, 4.102f, 4.103f, 4.104f, 4.105f, 4.106f, 4.107f, 4.108f, 4.108f, 4.109f, 4.11f, 4.111f, 4.112f, 4.113f, 4.114f, 4.115f, 4.115f, 4.116f, 4.117f, 4.118f, 4.119f, 4.12f, 4.121f, 4.121f, 4.122f, 4.123f, 4.124f, 4.125f, 4.125f, 4.126f, 4.127f, 4.128f, 4.129f, 4.129f, 4.13f, 4.131f, 4.132f, 4.132f, 4.133f, 4.134f, 4.135f, 4.135f, 4.136f, 4.137f, 4.138f, 4.138f, 4.139f, 4.14f, 4.14f, 4.141f, 4.142f, 4.143f, 4.143f, 4.144f, 4.145f, 4.146f, 4.146f, 4.147f, 4.148f, 4.149f, 4.149f, 4.15f, 4.151f, 4.152f, 4.152f, 4.153f, 4.154f, 4.155f, 4.155f, 4.156f, 4.157f, 4.158f, 4.159f, 4.159f, 4.16f, 4.161f, 4.162f, 4.163f, 4.163f, 4.164f, 4.165f, 4.166f, 4.167f, 4.168f, 4.169f, 4.17f, 4.17f, 4.171f, 4.172f, 4.173f, 4.174f, 4.175f, 4.176f, 4.177f, 4.178f, 4.179f, 4.18f, 4.181f, 4.182f, 4.183f, 4.184f, 4.185f, 4.187f, 4.188f, 4.189f, 4.19f, 4.191f, 4.192f, 4.193f, 4.195f, 4.196f, 4.197f, 4.198f, 4.199f, 4.201f, 4.202f, 4.203f, 4.205f, 4.206f, 4.207f, 4.209f, 4.21f, 4.212f, 4.213f, 4.215f, 4.216f};

static const float OCVdischarge[1001]    = {3.261f, 3.267f, 3.272f, 3.278f, 3.283f, 3.288f, 3.294f, 3.299f, 3.303f, 3.308f, 3.313f, 3.317f, 3.322f, 3.326f, 3.33f, 3.335f, 3.339f, 3.343f, 3.346f, 3.35f, 3.354f, 3.357f, 3.361f, 3.364f, 3.367f, 3.37f, 3.374f, 3.377f, 3.379f, 3.382f, 3.385f, 3.388f, 3.39f, 3.393f, 3.395f, 3.398f, 3.4f, 3.402f, 3.404f, 3.406f, 3.408f, 3.41f, 3.412f, 3.414f, 3.416f, 3.417f, 3.419f, 3.421f, 3.422f, 3.424f, 3.425f, 3.426f, 3.428f, 3.429f, 3.43f, 3.431f, 3.433f, 3.434f, 3.435f, 3.436f, 3.437f, 3.438f, 3.439f, 3.439f, 3.44f, 3.441f, 3.442f, 3.443f, 3.443f, 3.444f, 3.445f, 3.445f, 3.446f, 3.447f, 3.447f, 3.448f, 3.449f, 3.449f, 3.45f, 3.45f, 3.451f, 3.451f, 3.452f, 3.453f, 3.453f, 3.454f, 3.454f, 3.455f, 3.455f, 3.456f, 3.456f, 3.457f, 3.458f, 3.458f, 3.459f, 3.46f, 3.46f, 3.461f, 3.462f, 3.462f, 3.463f, 3.464f, 3.465f, 3.465f, 3.466f, 3.467f, 3.468f, 3.469f, 3.47f, 3.471f, 3.472f, 3.473f, 3.474f, 3.475f, 3.476f, 3.477f, 3.478f, 3.479f, 3.48f, 3.481f, 3.482f, 3.483f, 3.484f, 3.485f, 3.487f, 3.488f, 3.489f, 3.49f, 3.491f, 3.492f, 3.494f, 3.495f, 3.496f, 3.497f, 3.498f, 3.499f, 3.501f, 3.502f, 3.503f, 3.504f, 3.505f, 3.507f, 3.508f, 3.509f, 3.51f, 3.511f, 3.512f, 3.514f, 3.515f, 3.516f, 3.517f, 3.518f, 3.519f, 3.52f, 3.521f, 3.522f, 3.524f, 3.525f, 3.526f, 3.527f, 3.528f, 3.529f, 3.53f, 3.531f, 3.532f, 3.533f, 3.534f, 3.535f, 3.536f, 3.537f, 3.538f, 3.539f, 3.539f, 3.54f, 3.541f, 3.542f, 3.543f, 3.544f, 3.545f, 3.546f, 3.547f, 3.548f, 3.548f, 3.549f, 3.55f, 3.551f, 3.552f, 3.553f, 3.553f, 3.554f, 3.555f, 3.556f, 3.557f, 3.558f, 3.558f, 3.559f, 3.56f, 3.561f, 3.561f, 3.562f, 3.563f, 3.564f, 3.565f, 3.565f, 3.566f, 3.567f, 3.568f, 3.568f, 3.569f, 3.57f, 3.57f, 3.571f, 3.572f, 3.573f, 3.573f, 3.574f, 3.575f, 3.575f, 3.576f, 3.577f, 3.577f, 3.578f, 3.579f, 3.579f, 3.58f, 3.581f, 3.581f, 3.582f, 3.583f, 3.583f, 3.584f, 3.585f, 3.585f, 3.586f, 3.587f, 3.587f, 3.588f, 3.588f, 3.589f, 3.59f, 3.59f, 3.591f, 3.591f, 3.592f, 3.593f, 3.593f, 3.594f, 3.594f, 3.595f, 3.595f, 3.596f, 3.597f, 3.597f, 3.598f, 3.598f, 3.599f, 3.599f, 3.6f, 3.6f, 3.601f, 3.601f, 3.602f, 3.602f, 3.603f, 3.603f, 3.604f, 3.604f, 3.605f, 3.605f, 3.606f, 3.606f, 3.607f, 3.607f, 3.608f, 3.608f, 3.609f, 3.609f, 3.61f, 3.61f, 3.611f, 3.611f, 3.612f, 3.612f, 3.613f, 3.613f, 3.614f, 3.614f, 3.614f, 3.615f, 3.615f, 3.616f, 3.616f, 3.617f, 3.617f, 3.617f, 3.618f, 3.618f, 3.619f, 3.619f, 3.62f, 3.62f, 3.62f, 3.621f, 3.621f, 3.622f, 3.622f, 3.622f, 3.623f, 3.623f, 3.624f, 3.624f, 3.624f, 3.625f, 3.625f, 3.626f, 3.626f, 3.626f, 3.627f, 3.627f, 3.628f, 3.628f, 3.628f, 3.629f, 3.629f, 3.629f, 3.63f, 3.63f, 3.631f, 3.631f, 3.631f, 3.632f, 3.632f, 3.632f, 3.633f, 3.633f, 3.634f, 3.634f, 3.634f, 3.635f, 3.635f, 3.635f, 3.636f, 3.636f, 3.636f, 3.637f, 3.637f, 3.638f, 3.638f, 3.638f, 3.639f, 3.639f, 3.639f, 3.64f, 3.64f, 3.64f, 3.641f, 3.641f, 3.641f, 3.642f, 3.642f, 3.642f, 3.643f, 3.643f, 3.643f, 3.644f, 3.644f, 3.644f, 3.645f, 3.645f, 3.645f, 3.646f, 3.646f, 3.646f, 3.647f, 3.647f, 3.648f, 3.648f, 3.648f, 3.649f, 3.649f, 3.649f, 3.65f, 3.65f, 3.65f, 3.651f, 3.651f, 3.651f, 3.652f, 3.652f, 3.652f, 3.653f, 3.653f, 3.653f, 3.654f, 3.654f, 3.654f, 3.655f, 3.655f, 3.655f, 3.656f, 3.656f, 3.656f, 3.657f, 3.657f, 3.657f, 3.658f, 3.658f, 3.658f, 3.659f, 3.659f, 3.659f, 3.66f, 3.66f, 3.661f, 3.661f, 3.661f, 3.662f, 3.662f, 3.662f, 3.663f, 3.663f, 3.663f, 3.664f, 3.664f, 3.665f, 3.665f, 3.665f, 3.666f, 3.666f, 3.667f, 3.667f, 3.667f, 3.668f, 3.668f, 3.669f, 3.669f, 3.669f, 3.67f, 3.67f, 3.671f, 3.671f, 3.672f, 3.672f, 3.672f, 3.673f, 3.673f, 3.674f, 3.674f, 3.675f, 3.675f, 3.676f, 3.676f, 3.677f, 3.677f, 3.678f, 3.678f, 3.679f, 3.679f, 3.68f, 3.68f, 3.681f, 3.681f, 3.682f, 3.683f, 3.683f, 3.684f, 3.684f, 3.685f, 3.686f, 3.686f, 3.687f, 3.687f, 3.688f, 3.689f, 3.689f, 3.69f, 3.691f, 3.691f, 3.692f, 3.693f, 3.693f, 3.694f, 3.695f, 3.695f, 3.696f, 3.697f, 3.698f, 3.698f, 3.699f, 3.7f, 3.701f, 3.701f, 3.702f, 3.703f, 3.704f, 3.705f, 3.706f, 3.706f, 3.707f, 3.708f, 3.709f, 3.71f, 3.711f, 3.712f, 3.713f, 3.714f, 3.714f, 3.715f, 3.716f, 3.717f, 3.718f, 3.719f, 3.72f, 3.721f, 3.722f, 3.723f, 3.724f, 3.725f, 3.726f, 3.727f, 3.728f, 3.73f, 3.731f, 3.732f, 3.733f, 3.734f, 3.735f, 3.736f, 3.737f, 3.738f, 3.739f, 3.741f, 3.742f, 3.743f, 3.744f, 3.745f, 3.746f, 3.747f, 3.749f, 3.75f, 3.751f, 3.752f, 3.753f, 3.754f, 3.756f, 3.757f, 3.758f, 3.759f, 3.76f, 3.762f, 3.763f, 3.764f, 3.765f, 3.766f, 3.768f, 3.769f, 3.77f, 3.771f, 3.772f, 3.774f, 3.775f, 3.776f, 3.777f, 3.779f, 3.78f, 3.781f, 3.782f, 3.783f, 3.785f, 3.786f, 3.787f, 3.788f, 3.79f, 3.791f, 3.792f, 3.793f, 3.795f, 3.796f, 3.797f, 3.798f, 3.799f, 3.801f, 3.802f, 3.803f, 3.804f, 3.805f, 3.807f, 3.808f, 3.809f, 3.81f, 3.811f, 3.813f, 3.814f, 3.815f, 3.816f, 3.817f, 3.818f, 3.82f, 3.821f, 3.822f, 3.823f, 3.824f, 3.825f, 3.826f, 3.828f, 3.829f, 3.83f, 3.831f, 3.832f, 3.833f, 3.834f, 3.835f, 3.836f, 3.837f, 3.838f, 3.84f, 3.841f, 3.842f, 3.843f, 3.844f, 3.845f, 3.846f, 3.847f, 3.848f, 3.849f, 3.85f, 3.851f, 3.852f, 3.853f, 3.854f, 3.855f, 3.856f, 3.857f, 3.858f, 3.859f, 3.86f, 3.861f, 3.862f, 3.863f, 3.864f, 3.865f, 3.866f, 3.867f, 3.868f, 3.869f, 3.87f, 3.871f, 3.872f, 3.873f, 3.874f, 3.875f, 3.876f, 3.877f, 3.878f, 3.879f, 3.88f, 3.881f, 3.882f, 3.883f, 3.884f, 3.884f, 3.885f, 3.886f, 3.887f, 3.888f, 3.889f, 3.89f, 3.891f, 3.892f, 3.893f, 3.894f, 3.895f, 3.896f, 3.897f, 3.898f, 3.899f, 3.899f, 3.9f, 3.901f, 3.902f, 3.903f, 3.904f, 3.905f, 3.906f, 3.907f, 3.908f, 3.909f, 3.91f, 3.911f, 3.912f, 3.913f, 3.914f, 3.914f, 3.915f, 3.916f, 3.917f, 3.918f, 3.919f, 3.92f, 3.921f, 3.922f, 3.923f, 3.924f, 3.925f, 3.926f, 3.927f, 3.928f, 3.929f, 3.93f, 3.931f, 3.932f, 3.933f, 3.933f, 3.934f, 3.935f, 3.936f, 3.937f, 3.938f, 3.939f, 3.94f, 3.941f, 3.942f, 3.943f, 3.944f, 3.945f, 3.946f, 3.947f, 3.948f, 3.949f, 3.95f, 3.951f, 3.952f, 3.953f, 3.954f, 3.955f, 3.956f, 3.957f, 3.958f, 3.959f, 3.96f, 3.961f, 3.962f, 3.963f, 3.964f, 3.965f, 3.966f, 3.967f, 3.968f, 3.969f, 3.97f, 3.971f, 3.972f, 3.973f, 3.974f, 3.975f, 3.976f, 3.977f, 3.978f, 3.979f, 3.98f, 3.981f, 3.982f, 3.983f, 3.984f, 3.985f, 3.986f, 3.987f, 3.988f, 3.989f, 3.99f, 3.991f, 3.992f, 3.994f, 3.995f, 3.996f, 3.997f, 3.998f, 3.999f, 4.0f, 4.001f, 4.002f, 4.003f, 4.004f, 4.005f, 4.006f, 4.007f, 4.008f, 4.009f, 4.01f, 4.011f, 4.012f, 4.013f, 4.014f, 4.016f, 4.017f, 4.018f, 4.019f, 4.02f, 4.021f, 4.022f, 4.023f, 4.024f, 4.025f, 4.026f, 4.027f, 4.028f, 4.029f, 4.031f, 4.032f, 4.033f, 4.034f, 4.035f, 4.036f, 4.037f, 4.038f, 4.039f, 4.04f, 4.041f, 4.042f, 4.043f, 4.044f, 4.046f, 4.047f, 4.048f, 4.049f, 4.05f, 4.051f, 4.052f, 4.053f, 4.054f, 4.055f, 4.056f, 4.057f, 4.058f, 4.059f, 4.061f, 4.062f, 4.063f, 4.064f, 4.065f, 4.066f, 4.067f, 4.068f, 4.069f, 4.07f, 4.071f, 4.072f, 4.073f, 4.074f, 4.075f, 4.076f, 4.077f, 4.078f, 4.079f, 4.08f, 4.081f, 4.082f, 4.083f, 4.084f, 4.085f, 4.086f, 4.087f, 4.088f, 4.089f, 4.09f, 4.091f, 4.092f, 4.093f, 4.094f, 4.095f, 4.096f, 4.097f, 4.098f, 4.099f, 4.1f, 4.101f, 4.102f, 4.103f, 4.104f, 4.105f, 4.106f, 4.107f, 4.108f, 4.108f, 4.109f, 4.11f, 4.111f, 4.112f, 4.113f, 4.114f, 4.115f, 4.115f, 4.116f, 4.117f, 4.118f, 4.119f, 4.12f, 4.121f, 4.121f, 4.122f, 4.123f, 4.124f, 4.125f, 4.125f, 4.126f, 4.127f, 4.128f, 4.129f, 4.129f, 4.13f, 4.131f, 4.132f, 4.132f, 4.133f, 4.134f, 4.135f, 4.135f, 4.136f, 4.137f, 4.138f, 4.138f, 4.139f, 4.14f, 4.14f, 4.141f, 4.142f, 4.143f, 4.143f, 4.144f, 4.145f, 4.146f, 4.146f, 4.147f, 4.148f, 4.149f, 4.149f, 4.15f, 4.151f, 4.152f, 4.152f, 4.153f, 4.154f, 4.155f, 4.155f, 4.156f, 4.157f, 4.158f, 4.159f, 4.159f, 4.16f, 4.161f, 4.162f, 4.163f, 4.163f, 4.164f, 4.165f, 4.166f, 4.167f, 4.168f, 4.169f, 4.17f, 4.17f, 4.171f, 4.172f, 4.173f, 4.174f, 4.175f, 4.176f, 4.177f, 4.178f, 4.179f, 4.18f, 4.181f, 4.182f, 4.183f, 4.184f, 4.185f, 4.187f, 4.188f, 4.189f, 4.19f, 4.191f, 4.192f, 4.193f, 4.195f, 4.196f, 4.197f, 4.198f, 4.199f, 4.201f, 4.202f, 4.203f, 4.205f, 4.206f, 4.207f, 4.209f, 4.21f, 4.212f, 4.213f, 4.215f, 4.216f};


static const char batteryText[]       = "LG-E60-8S";

#endif /* LG_E60_8S_BATTERY_INFO_H */
/** @} */
