/*
 * E27 single-cell battery calibration data.
 *
 * Values are preserved from the supplied MATLAB/Simulink generated model.
 * The estimator implementation is now an analytic EKF optimized for Teensy 4.1.
 */

#include "E27_SingleCell_Model.h"

/* Block parameters (default storage) */
P_E27_SingleCell_Model_T E27_SingleCell_Model_P = {
  /* Nominal cell capacity [Ah]. Used to convert current into SoC change. */
  27.0f,

  /* Process-noise square-root matrix. The EKF converts this to Q at refresh. */
  { 1.0E-6f, 0.0f, 0.0f, 0.0f, 1.0E-6f, 0.0f, 0.0f, 0.0f, 1.0E-6f },

  /* Voltage-measurement noise standard deviation. The EKF uses its square as R. */
  0.1f,

  /* R1 lookup table [ohm]: 7 SoC points x 3 temperature points. */
  { 0.0109f, 0.0069f, 0.0047f, 0.0034f, 0.0033f, 0.0033f, 0.0028f, 0.0029f, 0.0024f,
    0.0026f, 0.0016f, 0.0023f, 0.0018f, 0.0017f, 0.0013f, 0.0012f, 0.0013f, 0.001f,
    0.0014f, 0.0011f, 0.0011f },

  /* Shared SoC breakpoints for R1, C1, and OCV tables [0..1]. */
  { 0.0f, 0.1f, 0.25f, 0.5f, 0.75f, 0.9f, 1.0f },

  /* Shared temperature breakpoints for R1, C1, and OCV tables [K]. */
  { 278.0f, 293.0f, 313.0f },

  /* Open-circuit-voltage lookup table [V]: 7 SoC x 3 temperature. */
  { 3.49f, 3.55f, 3.62f, 3.71f, 3.91f, 4.07f, 4.19f, 3.5f, 3.57f, 3.63f, 3.71f, 3.93f, 4.08f,
    4.19f, 3.51f, 3.56f, 3.64f, 3.72f, 3.94f, 4.08f, 4.19f },

  /* Start of '<S1>/CoreSubsys' */
  {
    /* Legacy UKF covariance weights retained for parameter-layout compatibility. */
    { 2.0f, 0.16666666666666666f, 0.16666666666666666f, 0.16666666666666666f,
      0.16666666666666666f, 0.16666666666666666f, 0.16666666666666666f },

    /* Legacy UKF covariance weights retained for parameter-layout compatibility. */
    { 2.0f, 0.16666666666666666f, 0.16666666666666666f, 0.16666666666666666f,
      0.16666666666666666f, 0.16666666666666666f, 0.16666666666666666f },

    /* Generated covariance-update count. Fixed at one in this model. */
    1.0f,

    /* Legacy generated diag(Wc) matrix retained for ABI compatibility.
     * The optimized hot path uses the seven scalar Wc values above instead. */
    { 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.16666666666666666f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.16666666666666666f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.16666666666666666f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.16666666666666666f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.16666666666666666f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.16666666666666666f },

    /* Legacy current-sign gain (-1); retained for generated ABI compatibility. */
    -1.0f,

    /* Initial RC polarization voltage [V]. */
    0.0f,

    /* Legacy generated ones vector; retained for ABI compatibility. */
    { 1.0f, 1.0f, 1.0f },

    /* Initial 3x3 square-root covariance. Converted to P during EKF initialization. */
    { 0.0031622776601683794f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
      0.0031622776601683794f },

    /* Legacy UKF sigma spread factor retained for parameter-layout compatibility. */
    1.7320508075688772f,

    /* Legacy zero sigma-point template; retained for ABI compatibility. */
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },

    /* Start of '<S21>/CoreSubsys' */
    {
      /* Legacy UKF mean weights retained for parameter-layout compatibility. */
      { 0.0f, 0.16666666666666666f, 0.16666666666666666f, 0.16666666666666666f,
        0.16666666666666666f, 0.16666666666666666f, 0.16666666666666666f },

      /* Last valid {SoC, temperature} indices for the OCV table. */
      { 6U, 2U }
    }
    ,

    /* End of '<S21>/CoreSubsys' */

    /* Start of '<S22>/CoreSubsys' */
    {
      /* C1 lookup table [F]: 7 SoC points x 3 temperature points. */
      { 1834.8623853211009f, 4492.753623188406f, 23191.489361702126f,
        10588.235294117647f, 17878.78787878788f, 12121.212121212122f,
        8928.57142857143f, 12413.793103448277f, 18750.0f, 40384.615384615383f,
        18125.0f, 33478.260869565216f, 18333.333333333332f, 22941.176470588238f,
        30000.0f, 32500.000000000004f, 46923.076923076922f, 26000.0f,
        47857.142857142855f, 26363.63636363636f, 30000.0f },

      /* Legacy zero state offset retained for generated ABI compatibility. */
      0.0f,

      /* Legacy UKF mean weights retained for parameter-layout compatibility. */
      { 0.0f, 0.16666666666666666f, 0.16666666666666666f, 0.16666666666666666f,
        0.16666666666666666f, 0.16666666666666666f, 0.16666666666666666f },

      /* Last valid {SoC, temperature} indices for the R1 table. */
      { 6U, 2U },

      /* Last valid {SoC, temperature} indices for the C1 table. */
      { 6U, 2U }
    }
    /* End of '<S22>/CoreSubsys' */
  }
  /* End of '<S1>/CoreSubsys' */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
