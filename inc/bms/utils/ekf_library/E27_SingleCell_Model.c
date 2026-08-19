/*
 * E27 single-cell analytic EKF optimized for Teensy 4.1.
 *
 * State vector:
 *   x0 = state of charge
 *   x1 = RC polarization voltage
 *   x2 = ohmic resistance R0
 *
 * The filter uses analytic Jacobians of the exact discrete state equations
 * present in the supplied generated model.  Because the R1, C1, and OCV maps
 * are piecewise bilinear, their SoC derivatives are obtained directly from the
 * active interpolation segment.  No numerical differentiation, sigma points,
 * matrix decomposition, or general-purpose matrix routines are required.
 */

#include "E27_SingleCell_Model.h"

B_E27_SingleCell_Model_T E27_SingleCell_Model_B;
DW_E27_SingleCell_Model_T E27_SingleCell_Model_DW;
ExtU_E27_SingleCell_Model_T E27_SingleCell_Model_U;
ExtY_E27_SingleCell_Model_T E27_SingleCell_Model_Y;

static RT_MODEL_E27_SingleCell_Model_T E27_SingleCell_Model_M_;
RT_MODEL_E27_SingleCell_Model_T *const E27_SingleCell_Model_M =
  &E27_SingleCell_Model_M_;

#define E27_SOC_POINTS             (7U)
#define E27_MIN_POSITIVE_VALUE     (1.0e-20f)

typedef struct {
  uint32_t index;
  float fraction;
  float invSpan;
  bool isClipped;
} E27_Prelookup_T;

typedef struct {
  float value;
  float dSoc;
} E27_TableValueSlope_T;

typedef struct {
  float socCurrentScale;
  float measurementVariance;
  float q00;
  float q01;
  float q02;
  float q11;
  float q12;
  float q22;
  float socInvSpan[6];
  float tempInvSpan[2];
} E27_CalibrationCache_T;

static E27_CalibrationCache_T E27_CalibrationCache;

static const char E27_ErrorCapacity[] = "E27 EKF: nominal capacity must be positive";
static const char E27_ErrorRC[] = "E27 EKF: R1 and C1 calibration must be positive";
static const char E27_ErrorInnovation[] = "E27 EKF: invalid innovation covariance";

#if defined(__GNUC__)
#define E27_ALWAYS_INLINE static inline __attribute__((always_inline))
#else
#define E27_ALWAYS_INLINE static inline
#endif

/* Locate a clipped breakpoint interval.  Starting from the previous interval
 * makes the common case very cheap because SoC and temperature change slowly.
 */
E27_ALWAYS_INLINE E27_Prelookup_T E27_prelookup(float u, const float bp[],
  const float invSpan[], uint32_t maxIndex, uint32_t previousIndex)
{
  E27_Prelookup_T result;
  uint32_t index;

  if (u < bp[0U]) {
    result.index = 0U;
    result.fraction = 0.0f;
    result.invSpan = 0.0f;
    result.isClipped = true;
    return result;
  }

  if (u > bp[maxIndex]) {
    result.index = maxIndex - 1U;
    result.fraction = 1.0f;
    result.invSpan = 0.0f;
    result.isClipped = true;
    return result;
  }

  index = (previousIndex < maxIndex) ? previousIndex : 0U;

  while ((index > 0U) && (u < bp[index])) {
    --index;
  }

  while (((index + 1U) < maxIndex) && (u >= bp[index + 1U])) {
    ++index;
  }

  result.index = index;
  result.invSpan = invSpan[index];
  result.fraction = (u - bp[index]) * result.invSpan;
  result.isClipped = false;
  return result;
}

E27_ALWAYS_INLINE float E27_lerp(float lower, float upper, float fraction)
{
  return lower + (upper - lower) * fraction;
}

/* Evaluate one 7x3 table and its analytic SoC derivative.  Temperature is
 * interpolated first at the two active SoC breakpoints.  Within a breakpoint
 * interval the resulting SoC relationship is linear, so the derivative is
 * simply the segment slope.
 */
E27_ALWAYS_INLINE E27_TableValueSlope_T E27_lookup_value_and_soc_slope(
  const float table[21], const E27_Prelookup_T socLookup,
  const E27_Prelookup_T tempLookup)
{
  E27_TableValueSlope_T result;
  const uint32_t socIndex = socLookup.index;
  const uint32_t lowerBase = tempLookup.index * E27_SOC_POINTS;
  const uint32_t upperBase = lowerBase + E27_SOC_POINTS;
  const float lowerSocValue = E27_lerp(table[lowerBase + socIndex],
    table[upperBase + socIndex], tempLookup.fraction);
  const float upperSocValue = E27_lerp(table[lowerBase + socIndex + 1U],
    table[upperBase + socIndex + 1U], tempLookup.fraction);
  result.value = E27_lerp(lowerSocValue, upperSocValue, socLookup.fraction);
  result.dSoc = (!socLookup.isClipped) ?
    ((upperSocValue - lowerSocValue) * socLookup.invSpan) : 0.0f;
  return result;
}

/* Convert a generated 3x3 square-root covariance S into P = S*S'.  The
 * generated arrays are column-major, matching the sigma-point columns used by
 * the original implementation.
 */
static void E27_square_root_to_covariance(const float s[9], float p[9])
{
  uint32_t row;
  uint32_t col;
  uint32_t k;

  for (row = 0U; row < 3U; ++row) {
    for (col = 0U; col < 3U; ++col) {
      float sum = 0.0f;
      for (k = 0U; k < 3U; ++k) {
        sum += s[row + 3U * k] * s[col + 3U * k];
      }
      p[3U * row + col] = sum;
    }
  }
}

/* Recompute quantities derived from calibration.  Call this after changing
 * nominal capacity, process-noise matrix, or voltage-noise standard deviation.
 */
void E27_SingleCell_Model_refresh_calibration_cache(void)
{
  float q[9];
  const float capacity = E27_SingleCell_Model_P.SOCEstimatorAdaptiveKalmanFilte;
  const float voltageNoise = E27_SingleCell_Model_P.SOCEstimatorAdaptiveKalmanFil_l;

  if (capacity > E27_MIN_POSITIVE_VALUE) {
    E27_CalibrationCache.socCurrentScale = -1.0f / (capacity * 3600.0f);
  } else {
    E27_CalibrationCache.socCurrentScale = 0.0f;
    rtmSetErrorStatus(E27_SingleCell_Model_M, E27_ErrorCapacity);
  }

  E27_CalibrationCache.measurementVariance = voltageNoise * voltageNoise;

  {
    uint32_t i;
    for (i = 0U; i < 6U; ++i) {
      const float span =
        E27_SingleCell_Model_P.SOCEstimatorAdaptiveKalmanFil_e[i + 1U] -
        E27_SingleCell_Model_P.SOCEstimatorAdaptiveKalmanFil_e[i];
      E27_CalibrationCache.socInvSpan[i] =
        (span > E27_MIN_POSITIVE_VALUE) ? (1.0f / span) : 0.0f;
    }
    for (i = 0U; i < 2U; ++i) {
      const float span =
        E27_SingleCell_Model_P.SOCEstimatorAdaptiveKalmanFil_j[i + 1U] -
        E27_SingleCell_Model_P.SOCEstimatorAdaptiveKalmanFil_j[i];
      E27_CalibrationCache.tempInvSpan[i] =
        (span > E27_MIN_POSITIVE_VALUE) ? (1.0f / span) : 0.0f;
    }
  }

  E27_square_root_to_covariance(
    E27_SingleCell_Model_P.SOCEstimatorAdaptiveKalmanFil_a, q);

  E27_CalibrationCache.q00 = q[0];
  E27_CalibrationCache.q01 = q[1];
  E27_CalibrationCache.q02 = q[2];
  E27_CalibrationCache.q11 = q[4];
  E27_CalibrationCache.q12 = q[5];
  E27_CalibrationCache.q22 = q[8];
}

/* One EKF predict/correct update. */
void E27_SingleCell_Model_step(void)
{
  DW_CoreSubsys_E27_SingleCell__T *const dw =
    &E27_SingleCell_Model_DW.CoreSubsys[0];
  float *const p = dw->UnitDelayP_DSTATE;

  E27_Prelookup_T tempLookup;
  E27_Prelookup_T stateSocLookup;
  E27_Prelookup_T measurementSocLookup;
  E27_TableValueSlope_T r1Lookup;
  E27_TableValueSlope_T c1Lookup;
  E27_TableValueSlope_T ocvLookup;

  float x0;
  float x1;
  float x2;
  float currentModel;
  float r1;
  float c1;
  float invR1;
  float invC1;
  float invR1C1;
  float f10;
  float f11;

  float p00;
  float p01;
  float p02;
  float p11;
  float p12;
  float p22;
  float pp00;
  float pp01;
  float pp02;
  float pp11;
  float pp12;
  float pp22;

  float h0;
  float h2;
  float ph0;
  float ph1;
  float ph2;
  float innovationVariance;
  float invInnovationVariance;
  float innovation;
  float predictedVoltage;

  if (dw->icLoad) {
    dw->Delay_DSTATE[0] = E27_SingleCell_Model_U.temp_p;
    dw->Delay_DSTATE[1] = 0.0f;
    dw->Delay_DSTATE[2] = E27_SingleCell_Model_U.temp_g;
    dw->icLoad = false;
  }

  x0 = dw->Delay_DSTATE[0];
  x1 = dw->Delay_DSTATE[1];
  x2 = dw->Delay_DSTATE[2];

  /* The generated model applies a -1 current gain before both the RC state
   * equation and terminal-voltage equation.  Preserve that sign convention.
   */
  currentModel = -E27_SingleCell_Model_U.Current;

  tempLookup = E27_prelookup(E27_SingleCell_Model_U.temp,
    E27_SingleCell_Model_P.SOCEstimatorAdaptiveKalmanFil_j,
    E27_CalibrationCache.tempInvSpan,
    E27_SingleCell_Model_P.CoreSubsys.CoreSubsys.R1_maxIndex[1U],
    dw->temperatureIndex);
  dw->temperatureIndex = tempLookup.index;

  stateSocLookup = E27_prelookup(x0,
    E27_SingleCell_Model_P.SOCEstimatorAdaptiveKalmanFil_e,
    E27_CalibrationCache.socInvSpan,
    E27_SingleCell_Model_P.CoreSubsys.CoreSubsys.R1_maxIndex[0U],
    dw->stateSocIndex);
  dw->stateSocIndex = stateSocLookup.index;

  r1Lookup = E27_lookup_value_and_soc_slope(
    E27_SingleCell_Model_P.SOCEstimatorAdaptiveKalmanFil_n,
    stateSocLookup, tempLookup);
  c1Lookup = E27_lookup_value_and_soc_slope(
    E27_SingleCell_Model_P.CoreSubsys.CoreSubsys.C1_tableData,
    stateSocLookup, tempLookup);

  r1 = r1Lookup.value;
  c1 = c1Lookup.value;

  if (!((r1 > E27_MIN_POSITIVE_VALUE) && (c1 > E27_MIN_POSITIVE_VALUE))) {
    rtmSetErrorStatus(E27_SingleCell_Model_M, E27_ErrorRC);
    E27_SingleCell_Model_Y.SoCEst = x0;
    E27_SingleCell_Model_Y.R0Est = x2;
    return;
  }

  invR1 = 1.0f / r1;
  invC1 = 1.0f / c1;
  invR1C1 = invR1 * invC1;

  /* Predict the physical states using the same one-second forward-Euler
   * equations as the supplied generated model.
   */
  f11 = 1.0f - invR1C1;
  dw->Delay_DSTATE[0] = x0 +
    E27_CalibrationCache.socCurrentScale * currentModel;
  dw->Delay_DSTATE[1] = f11 * x1 + currentModel * invC1;
  dw->Delay_DSTATE[2] = x2;

  /* Analytic state Jacobian.  Only the RC-voltage row depends on SoC. */
  f10 = (-currentModel * c1Lookup.dSoc * invC1 * invC1) +
    (x1 * invR1C1 *
     (r1Lookup.dSoc * invR1 + c1Lookup.dSoc * invC1));

  /* Load the six independent entries of the symmetric covariance. */
  p00 = p[0];
  p01 = p[1];
  p02 = p[2];
  p11 = p[4];
  p12 = p[5];
  p22 = p[8];

  /* P- = F*P*F' + Q, expanded for
   * F = [1 0 0; f10 f11 0; 0 0 1].
   */
  pp00 = p00 + E27_CalibrationCache.q00;
  pp01 = f10 * p00 + f11 * p01 + E27_CalibrationCache.q01;
  pp02 = p02 + E27_CalibrationCache.q02;
  pp11 = f10 * f10 * p00 + (2.0f * f10 * f11 * p01) +
    f11 * f11 * p11 + E27_CalibrationCache.q11;
  pp12 = f10 * p02 + f11 * p12 + E27_CalibrationCache.q12;
  pp22 = p22 + E27_CalibrationCache.q22;

  /* Evaluate OCV and dOCV/dSoC at the predicted SoC. */
  measurementSocLookup = E27_prelookup(dw->Delay_DSTATE[0],
    E27_SingleCell_Model_P.SOCEstimatorAdaptiveKalmanFil_e,
    E27_CalibrationCache.socInvSpan,
    E27_SingleCell_Model_P.CoreSubsys.CoreSubsys_h.OCVTable_maxIndex[0U],
    dw->measurementSocIndex);
  dw->measurementSocIndex = measurementSocLookup.index;

  ocvLookup = E27_lookup_value_and_soc_slope(
    E27_SingleCell_Model_P.SOCEstimatorAdaptiveKalmanFil_k,
    measurementSocLookup, tempLookup);

  /* h(x) = OCV(SoC,T) - Vrc - R0*currentModel
   * H    = [dOCV/dSoC, -1, -currentModel]
   */
  h0 = ocvLookup.dSoc;
  h2 = -currentModel;
  predictedVoltage = (ocvLookup.value - dw->Delay_DSTATE[1]) -
    dw->Delay_DSTATE[2] * currentModel;

  /* PH' for scalar voltage measurement. */
  ph0 = pp00 * h0 - pp01 + pp02 * h2;
  ph1 = pp01 * h0 - pp11 + pp12 * h2;
  ph2 = pp02 * h0 - pp12 + pp22 * h2;

  innovationVariance = h0 * ph0 - ph1 + h2 * ph2 +
    E27_CalibrationCache.measurementVariance;

  if (!(innovationVariance > E27_MIN_POSITIVE_VALUE)) {
    /* Keep the valid prediction if the correction cannot be performed. */
    p[0] = pp00;
    p[1] = pp01;
    p[2] = pp02;
    p[3] = pp01;
    p[4] = pp11;
    p[5] = pp12;
    p[6] = pp02;
    p[7] = pp12;
    p[8] = pp22;

    E27_SingleCell_Model_Y.SoCEst = dw->Delay_DSTATE[0];
    E27_SingleCell_Model_Y.R0Est = dw->Delay_DSTATE[2];
    rtmSetErrorStatus(E27_SingleCell_Model_M, E27_ErrorInnovation);
    return;
  }

  invInnovationVariance = 1.0f / innovationVariance;
  innovation = E27_SingleCell_Model_U.CellVoltage - predictedVoltage;

  /* x+ = x- + K*r, with K = PH'/S. */
  dw->Delay_DSTATE[0] += ph0 * invInnovationVariance * innovation;
  dw->Delay_DSTATE[1] += ph1 * invInnovationVariance * innovation;
  dw->Delay_DSTATE[2] += ph2 * invInnovationVariance * innovation;

  /* P+ = P- - (PH')*(PH')'/S.
   * Computing only the six independent terms preserves exact symmetry while
   * avoiding general matrix multiplication.
   */
  p00 = pp00 - ph0 * ph0 * invInnovationVariance;
  p01 = pp01 - ph0 * ph1 * invInnovationVariance;
  p02 = pp02 - ph0 * ph2 * invInnovationVariance;
  p11 = pp11 - ph1 * ph1 * invInnovationVariance;
  p12 = pp12 - ph1 * ph2 * invInnovationVariance;
  p22 = pp22 - ph2 * ph2 * invInnovationVariance;

  /* Roundoff can occasionally produce a tiny negative diagonal term even when
   * the exact covariance is positive semidefinite.  A very small floor keeps
   * later updates well-conditioned without materially changing the estimate.
   */
  if (p00 < E27_MIN_POSITIVE_VALUE) {
    p00 = E27_MIN_POSITIVE_VALUE;
  }
  if (p11 < E27_MIN_POSITIVE_VALUE) {
    p11 = E27_MIN_POSITIVE_VALUE;
  }
  if (p22 < E27_MIN_POSITIVE_VALUE) {
    p22 = E27_MIN_POSITIVE_VALUE;
  }

  p[0] = p00;
  p[1] = p01;
  p[2] = p02;
  p[3] = p01;
  p[4] = p11;
  p[5] = p12;
  p[6] = p02;
  p[7] = p12;
  p[8] = p22;

  E27_SingleCell_Model_Y.SoCEst = dw->Delay_DSTATE[0];
  E27_SingleCell_Model_Y.R0Est = dw->Delay_DSTATE[2];
}

void E27_SingleCell_Model_initialize(void)
{
  float initialCovariance[9];

  E27_SingleCell_Model_B.reserved = 0U;
  E27_SingleCell_Model_DW.CoreSubsys[0].Delay_DSTATE[0] = 0.0f;
  E27_SingleCell_Model_DW.CoreSubsys[0].Delay_DSTATE[1] = 0.0f;
  E27_SingleCell_Model_DW.CoreSubsys[0].Delay_DSTATE[2] = 0.0f;
  E27_SingleCell_Model_DW.CoreSubsys[0].stateSocIndex = 0U;
  E27_SingleCell_Model_DW.CoreSubsys[0].measurementSocIndex = 0U;
  E27_SingleCell_Model_DW.CoreSubsys[0].temperatureIndex = 0U;
  E27_SingleCell_Model_Y.SoCEst = 0.0f;
  E27_SingleCell_Model_Y.R0Est = 0.0f;
  rtmSetErrorStatus(E27_SingleCell_Model_M, (const char *)0);

  E27_SingleCell_Model_refresh_calibration_cache();

  E27_square_root_to_covariance(
    E27_SingleCell_Model_P.CoreSubsys.UnitDelayP_InitialCondition,
    initialCovariance);
  {
    uint32_t i;
    for (i = 0U; i < 9U; ++i) {
      E27_SingleCell_Model_DW.CoreSubsys[0].UnitDelayP_DSTATE[i] =
        initialCovariance[i];
    }
  }

  E27_SingleCell_Model_DW.CoreSubsys[0].icLoad = true;
}

void E27_SingleCell_Model_terminate(void)
{
  /* No resources require explicit shutdown. */
}
