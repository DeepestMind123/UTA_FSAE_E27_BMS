/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: E27_SingleCell_Model.h
 *
 * Code generated for Simulink model 'E27_SingleCell_Model'.
 *
 * Model version                  : 1.23
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Jun 12 18:41:23 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef E27_SingleCell_Model_h_
#define E27_SingleCell_Model_h_
#ifndef E27_SingleCell_Model_COMMON_INCLUDES_
#define E27_SingleCell_Model_COMMON_INCLUDES_
#endif                               /* E27_SingleCell_Model_COMMON_INCLUDES_ */


/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#include <math.h>

#include "rtwtypes.h"
#include "util_single_cell_ekf_types.h"
#include "rt_nonfinite.h"
#include "m128d.h"
#include "rtwtypes.h"


/* Block signals for system '<S16>/State function' */
typedef struct {
  real_T Probe[2];                     /* '<S28>/Probe' */
} B_CoreSubsys_E27_SingleCell_c_T;

/* Block states (default storage) for system '<S16>/State function' */
typedef struct {
  uint32_T m_bpIndex[2];               /* '<S28>/R1' */
  uint32_T m_bpIndex_e[2];             /* '<S28>/C1' */
} DW_CoreSubsys_E27_SingleCel_a_T;

/* Block states (default storage) for system '<S16>/Measurement function' */
typedef struct {
  uint32_T m_bpIndex[2];               /* '<S26>/OCV Table' */
} DW_CoreSubsys_E27_SingleCe_ab_T;

/* Block signals for system '<Root>/SOC Estimator (Adaptive Kalman Filter)' */
typedef struct {
  real_T Assignment2[21];              /* '<S17>/Assignment2' */
  real_T ImpAsg_InsertedFor_xk1_at_inpor[21];/* '<S22>/State function' */
  B_CoreSubsys_E27_SingleCell_c_T CoreSubsys[7];/* '<S16>/State function' */
} B_CoreSubsys_E27_SingleCell_M_T;

/* Block states (default storage) for system '<Root>/SOC Estimator (Adaptive Kalman Filter)' */
typedef struct {
  real_T Delay_DSTATE[3];              /* '<S15>/Delay' */
  real_T UnitDelayP_DSTATE[9];         /* '<S13>/Unit Delay - P' */
  boolean_T icLoad;                    /* '<S15>/Delay' */
  DW_CoreSubsys_E27_SingleCe_ab_T CoreSubsys_h[7];/* '<S16>/Measurement function' */
  DW_CoreSubsys_E27_SingleCel_a_T CoreSubsys[7];/* '<S16>/State function' */
} DW_CoreSubsys_E27_SingleCell__T;

/* Block signals (default storage) */
typedef struct {
  B_CoreSubsys_E27_SingleCell_M_T CoreSubsys[1];
                           /* '<Root>/SOC Estimator (Adaptive Kalman Filter)' */
} B_E27_SingleCell_Model_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  DW_CoreSubsys_E27_SingleCell__T CoreSubsys[1];
                           /* '<Root>/SOC Estimator (Adaptive Kalman Filter)' */
} DW_E27_SingleCell_Model_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Pooled Parameter (Expression: Wc)
   * Referenced by:
   *   '<S16>/updatePx'
   *   '<S16>/updatePy'
   */
  real_T pooled1[7];

  /* Expression: Q
   * Referenced by: '<S16>/updatePx'
   */
  real_T updatePx_Q[9];

  /* Pooled Parameter (Expression: Wm)
   * Referenced by:
   *   '<S21>/Constant'
   *   '<S22>/Constant'
   */
  real_T pooled2[7];

  /* Expression: R1_mat
   * Referenced by: '<S28>/R1'
   */
  real_T R1_tableData[21];

  /* Expression: SOC_vec
   * Referenced by: '<S28>/R1'
   */
  real_T R1_bp01Data[7];

  /* Expression: T_vec
   * Referenced by: '<S28>/R1'
   */
  real_T R1_bp02Data[3];

  /* Expression: C1_mat
   * Referenced by: '<S28>/C1'
   */
  real_T C1_tableData[21];

  /* Expression: SOC_vec
   * Referenced by: '<S28>/C1'
   */
  real_T C1_bp01Data[7];

  /* Expression: T_vec
   * Referenced by: '<S28>/C1'
   */
  real_T C1_bp02Data[3];

  /* Expression: V0_mat
   * Referenced by: '<S26>/OCV Table'
   */
  real_T OCVTable_tableData[21];

  /* Expression: SOC_vec
   * Referenced by: '<S26>/OCV Table'
   */
  real_T OCVTable_bp01Data[7];

  /* Expression: T_vec
   * Referenced by: '<S26>/OCV Table'
   */
  real_T OCVTable_bp02Data[3];

  /* Expression: diag(Wc)
   * Referenced by: '<S16>/Constant'
   */
  real_T Constant_Value[49];

  /* Expression: chol(P0,'lower')
   * Referenced by: '<S13>/Unit Delay - P'
   */
  real_T UnitDelayP_InitialCondition[9];

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<S26>/OCV Table'
   *   '<S28>/C1'
   *   '<S28>/R1'
   */
  uint32_T pooled4[2];
} ConstP_E27_SingleCell_Model_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T Current;                      /* '<Root>/Current' */
  real_T CellVoltage;                  /* '<Root>/CellVoltage' */
  real_T temp;                         /* '<Root>/Measured Temperature' */
  real_T temp_p;                       /* '<Root>/Initial SoC' */
  real_T temp_g;                       /* '<Root>/Initial R0' */
} ExtU_E27_SingleCell_Model_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T SoCEst;                       /* '<Root>/SoC est' */
  real_T R0Est;                        /* '<Root>/R0 Est' */
} ExtY_E27_SingleCell_Model_T;

/* Real-time Model Data Structure */
struct tag_RTM_E27_SingleCell_Model_T {
  const char_T * volatile errorStatus;
};

/* Block signals (default storage) */
extern B_E27_SingleCell_Model_T E27_SingleCell_Model_B;

/* Block states (default storage) */
extern DW_E27_SingleCell_Model_T E27_SingleCell_Model_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_E27_SingleCell_Model_T E27_SingleCell_Model_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_E27_SingleCell_Model_T E27_SingleCell_Model_Y;

/* Constant parameters (default storage) */
extern const ConstP_E27_SingleCell_Model_T E27_SingleCell_Model_ConstP;

/* Model entry point functions */
extern void E27_SingleCell_Model_initialize(void);
extern void E27_SingleCell_Model_step(void);
extern void E27_SingleCell_Model_terminate(void);

/* Real-time Model object */
extern RT_MODEL_E27_SingleCell_Model_T *const E27_SingleCell_Model_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S19>/Data Type Duplicate' : Unused code path elimination
 * Block '<S20>/Data Type Duplicate' : Unused code path elimination
 * Block '<S25>/Data Type Duplicate' : Unused code path elimination
 * Block '<S27>/Data Type Duplicate' : Unused code path elimination
 * Block '<S29>/Data Type Duplicate' : Unused code path elimination
 * Block '<S30>/Data Type Duplicate' : Unused code path elimination
 * Block '<S31>/Data Type Duplicate' : Unused code path elimination
 * Block '<S19>/Conversion' : Eliminate redundant data type conversion
 * Block '<S20>/Conversion' : Eliminate redundant data type conversion
 * Block '<S25>/Conversion' : Eliminate redundant data type conversion
 * Block '<S27>/Conversion' : Eliminate redundant data type conversion
 * Block '<S29>/Conversion' : Eliminate redundant data type conversion
 * Block '<S13>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S13>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S13>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S13>/Rate Transition3' : Eliminated since input and output rates are identical
 * Block '<S13>/Rate Transition4' : Eliminated since input and output rates are identical
 * Block '<S13>/Reshape' : Reshape block reduction
 * Block '<S30>/Conversion' : Eliminate redundant data type conversion
 * Block '<S31>/Conversion' : Eliminate redundant data type conversion
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'E27_SingleCell_Model'
 * '<S1>'   : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)'
 * '<S2>'   : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Data type'
 * '<S3>'   : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Data type1'
 * '<S4>'   : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Data type2'
 * '<S5>'   : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Data type3'
 * '<S6>'   : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Data type4'
 * '<S7>'   : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter'
 * '<S8>'   : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Data type/Inherit DT'
 * '<S9>'   : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Data type1/Inherit DT'
 * '<S10>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Data type2/Inherit DT'
 * '<S11>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Data type3/Inherit DT'
 * '<S12>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Data type4/Inherit DT'
 * '<S13>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF'
 * '<S14>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Correction'
 * '<S15>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Delay - X'
 * '<S16>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Prediction'
 * '<S17>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Sigma points'
 * '<S18>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Correction/updatePk'
 * '<S19>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Delay - X/Data Type Conversion Inherited'
 * '<S20>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Prediction/Data Type Conversion Inherited'
 * '<S21>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Prediction/Measurement function'
 * '<S22>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Prediction/State function'
 * '<S23>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Prediction/updatePx'
 * '<S24>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Prediction/updatePy'
 * '<S25>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Prediction/Measurement function/Data Type Conversion Inherited'
 * '<S26>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Prediction/Measurement function/Measurement function'
 * '<S27>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Prediction/State function/Data Type Conversion Inherited'
 * '<S28>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Prediction/State function/State function'
 * '<S29>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Prediction/State function/State function/Data Type Conversion Inherited'
 * '<S30>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Sigma points/Data Type Conversion Inherited'
 * '<S31>'  : 'E27_SingleCell_Model/SOC Estimator (Adaptive Kalman Filter)/Kalman Filter/UKF/Sigma points/Data Type Conversion Inherited1'
 */
#endif                                 /* E27_SingleCell_Model_h_ */