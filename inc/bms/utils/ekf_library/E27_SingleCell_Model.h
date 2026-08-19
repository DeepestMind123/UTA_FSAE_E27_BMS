/*
 * E27 single-cell analytic EKF.
 *
 * Target: Teensy 4.1 / NXP i.MX RT1062 / ARM Cortex-M7
 * Arithmetic: single-precision float only
 *
 * The public inputs, outputs, and calibration parameter names match the
 * previously supplied generated model as closely as practical so the EKF can
 * replace the optimized UKF with minimal application changes.
 */

#ifndef E27_SingleCell_Model_h_
#define E27_SingleCell_Model_h_

#include <stdbool.h>
#include <stdint.h>
#include "E27_SingleCell_Model_types.h"

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* No block signal storage is required by the analytic EKF.  The object is
 * retained because existing application code may reference the generated
 * global symbol.
 */
typedef struct {
  uint8_t reserved;
} B_E27_SingleCell_Model_T;

/* Persistent estimator state.
 *
 * Delay_DSTATE:
 *   [0] SoC
 *   [1] RC polarization voltage
 *   [2] Ohmic resistance R0
 *
 * UnitDelayP_DSTATE stores the full symmetric 3x3 EKF covariance in row-major
 * form.  The previous UKF used this field for a square-root covariance; code
 * should therefore treat it as opaque estimator state rather than interpreting
 * its elements directly.
 */
typedef struct {
  float Delay_DSTATE[3];
  float UnitDelayP_DSTATE[9];
  uint32_t stateSocIndex;
  uint32_t measurementSocIndex;
  uint32_t temperatureIndex;
  bool icLoad;
} DW_CoreSubsys_E27_SingleCell__T;

typedef struct {
  DW_CoreSubsys_E27_SingleCell__T CoreSubsys[1];
} DW_E27_SingleCell_Model_T;

/* External inputs.  Current uses the same sign convention as the supplied
 * generated model.
 */
typedef struct {
  float Current;      /* Cell current [A] */
  float CellVoltage;  /* Measured terminal voltage [V] */
  float temp;         /* Measured cell temperature [K] */
  float temp_p;       /* Initial SoC estimate [0..1] */
  float temp_g;       /* Initial R0 estimate [ohm] */
} ExtU_E27_SingleCell_Model_T;

typedef struct {
  float SoCEst;       /* Estimated state of charge [0..1] */
  float R0Est;        /* Estimated ohmic resistance [ohm] */
} ExtY_E27_SingleCell_Model_T;

/* Legacy state-model calibration.  UKF weights remain in the parameter object
 * for drop-in source compatibility but are not used by the analytic EKF.
 */
struct P_CoreSubsys_E27_SingleCell_k_T_ {
  float C1_tableData[21];
  float Constant1_Value;
  float Constant_Value[7];
  uint32_t R1_maxIndex[2];
  uint32_t C1_maxIndex[2];
};

/* Legacy measurement-model calibration retained for compatibility. */
struct P_CoreSubsys_E27_SingleCel_kw_T_ {
  float Constant_Value[7];
  uint32_t OCVTable_maxIndex[2];
};

/* Legacy generated UKF parameter block.  Only the initial covariance,
 * process-noise square-root matrix, and physical calibration tables are
 * relevant to the EKF.  Remaining fields are intentionally retained so code
 * that writes the existing parameter structure still compiles unchanged.
 */
struct P_CoreSubsys_E27_SingleCell_M_T_ {
  float updatePy_Wc[7];
  float updatePx_Wc[7];
  float updatePk_m;
  float Constant_Value[49];
  float Gain_Gain;
  float Constant1_Value;
  float Constant1_Value_g[3];
  float UnitDelayP_InitialCondition[9];
  float Gain_Gain_n;
  float Constant_Value_o[21];
  P_CoreSubsys_E27_SingleCel_kw_T CoreSubsys_h;
  P_CoreSubsys_E27_SingleCell_k_T CoreSubsys;
};

/* User-visible calibration. */
struct P_E27_SingleCell_Model_T_ {
  float SOCEstimatorAdaptiveKalmanFilte;    /* Nominal capacity [Ah] */
  float SOCEstimatorAdaptiveKalmanFil_a[9]; /* Process-noise square-root matrix */
  float SOCEstimatorAdaptiveKalmanFil_l;    /* Voltage noise standard deviation [V] */
  float SOCEstimatorAdaptiveKalmanFil_n[21];/* R1 table [ohm], 7 SoC x 3 temp */
  float SOCEstimatorAdaptiveKalmanFil_e[7]; /* SoC breakpoints */
  float SOCEstimatorAdaptiveKalmanFil_j[3]; /* Temperature breakpoints [K] */
  float SOCEstimatorAdaptiveKalmanFil_k[21];/* OCV table [V], 7 SoC x 3 temp */
  P_CoreSubsys_E27_SingleCell_M_T CoreSubsys;
};

struct tag_RTM_E27_SingleCell_Model_T {
  const char * volatile errorStatus;
};

#ifdef __cplusplus
extern "C" {
#endif

extern P_E27_SingleCell_Model_T E27_SingleCell_Model_P;
extern B_E27_SingleCell_Model_T E27_SingleCell_Model_B;
extern DW_E27_SingleCell_Model_T E27_SingleCell_Model_DW;
extern ExtU_E27_SingleCell_Model_T E27_SingleCell_Model_U;
extern ExtY_E27_SingleCell_Model_T E27_SingleCell_Model_Y;
extern RT_MODEL_E27_SingleCell_Model_T *const E27_SingleCell_Model_M;

void E27_SingleCell_Model_initialize(void);
void E27_SingleCell_Model_refresh_calibration_cache(void);
void E27_SingleCell_Model_step(void);
void E27_SingleCell_Model_terminate(void);

#ifdef __cplusplus
}
#endif

#endif /* E27_SingleCell_Model_h_ */
