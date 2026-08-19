/*
 * E27 single-cell equivalent-circuit estimator types.
 *
 * This file is based on the MATLAB/Simulink generated interface supplied with
 * the project, but the estimator implementation has been replaced with a
 * hand-specialized analytic EKF for the Teensy 4.1 (i.MX RT1062 Cortex-M7).
 */

#ifndef E27_SingleCell_Model_types_h_
#define E27_SingleCell_Model_types_h_

/* Legacy calibration structures are retained so existing calibration code can
 * continue using the generated parameter object without field-layout changes.
 */
typedef struct P_CoreSubsys_E27_SingleCell_k_T_ P_CoreSubsys_E27_SingleCell_k_T;
typedef struct P_CoreSubsys_E27_SingleCel_kw_T_ P_CoreSubsys_E27_SingleCel_kw_T;
typedef struct P_CoreSubsys_E27_SingleCell_M_T_ P_CoreSubsys_E27_SingleCell_M_T;
typedef struct P_E27_SingleCell_Model_T_ P_E27_SingleCell_Model_T;

typedef struct tag_RTM_E27_SingleCell_Model_T RT_MODEL_E27_SingleCell_Model_T;

#endif /* E27_SingleCell_Model_types_h_ */
