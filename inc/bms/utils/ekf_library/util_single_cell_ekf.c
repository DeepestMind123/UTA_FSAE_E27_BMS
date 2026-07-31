/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: E27_SingleCell_Model.c
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

 #include "util_single_cell_ekf.h"


/* Block signals (default storage) */
B_E27_SingleCell_Model_T E27_SingleCell_Model_B;

/* Block states (default storage) */
DW_E27_SingleCell_Model_T E27_SingleCell_Model_DW;

/* External inputs (root inport signals with default storage) */
ExtU_E27_SingleCell_Model_T E27_SingleCell_Model_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_E27_SingleCell_Model_T E27_SingleCell_Model_Y;

/* Real-time model */
static RT_MODEL_E27_SingleCell_Model_T E27_SingleCell_Model_M_;
RT_MODEL_E27_SingleCell_Model_T *const E27_SingleCell_Model_M =
  &E27_SingleCell_Model_M_;

/* Forward declaration for local functions */
static real_T E27_SingleCell_Model_xnrm2_n(int32_T n, const real_T x[27],
  int32_T ix0);
static void E27_SingleCell_Model_xgemv(int32_T m, int32_T n, const real_T A[27],
  int32_T ia0, const real_T x[27], int32_T ix0, real_T y[3]);
static void E27_SingleCell_Model_xgerc(int32_T m, int32_T n, real_T alpha1,
  int32_T ix0, const real_T y[3], real_T A[27], int32_T ia0);
static void E27_SingleCell_Model_qr(const real_T A[27], real_T b_Q[27], real_T
  R[9]);
static void E27_SingleCell_Model_rotate(real_T x, real_T y, real_T *c, real_T *s,
  real_T *r);
static real_T E27_SingleCell_Model_xnrm2(int32_T n, const real_T x[7], int32_T
  ix0);
real_T look2_pbinlcpw(real_T u0, real_T u1, const real_T bp0[], const real_T
                      bp1[], const real_T table[], uint32_T prevIndex[], const
                      uint32_T maxIndex[], uint32_T stride)
{
  real_T fractions[2];
  real_T frac;
  real_T yL_0d0;
  real_T yL_0d1;
  uint32_T bpIndices[2];
  uint32_T bpIdx;
  uint32_T found;
  uint32_T iLeft;
  uint32_T iRght;

  /* Column-major Lookup 2-D
     Search method: 'binary'
     Use previous index: 'on'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    bpIdx = 0U;
    frac = 0.0;
  } else if (u0 < bp0[maxIndex[0U]]) {
    /* Binary Search using Previous Index */
    bpIdx = prevIndex[0U];
    iLeft = 0U;
    iRght = maxIndex[0U];
    found = 0U;
    while (found == 0U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx - 1U;
        bpIdx = ((bpIdx + iLeft) - 1U) >> 1U;
      } else if (u0 < bp0[bpIdx + 1U]) {
        found = 1U;
      } else {
        iLeft = bpIdx + 1U;
        bpIdx = ((bpIdx + iRght) + 1U) >> 1U;
      }
    }

    frac = (u0 - bp0[bpIdx]) / (bp0[bpIdx + 1U] - bp0[bpIdx]);
  } else {
    bpIdx = maxIndex[0U] - 1U;
    frac = 1.0;
  }

  prevIndex[0U] = bpIdx;
  fractions[0U] = frac;
  bpIndices[0U] = bpIdx;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 <= bp1[0U]) {
    bpIdx = 0U;
    frac = 0.0;
  } else if (u1 < bp1[maxIndex[1U]]) {
    /* Binary Search using Previous Index */
    bpIdx = prevIndex[1U];
    iLeft = 0U;
    iRght = maxIndex[1U];
    found = 0U;
    while (found == 0U) {
      if (u1 < bp1[bpIdx]) {
        iRght = bpIdx - 1U;
        bpIdx = ((bpIdx + iLeft) - 1U) >> 1U;
      } else if (u1 < bp1[bpIdx + 1U]) {
        found = 1U;
      } else {
        iLeft = bpIdx + 1U;
        bpIdx = ((bpIdx + iRght) + 1U) >> 1U;
      }
    }

    frac = (u1 - bp1[bpIdx]) / (bp1[bpIdx + 1U] - bp1[bpIdx]);
  } else {
    bpIdx = maxIndex[1U] - 1U;
    frac = 1.0;
  }

  prevIndex[1U] = bpIdx;

  /* Column-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  iLeft = bpIdx * stride + bpIndices[0U];
  yL_0d0 = table[iLeft];
  yL_0d0 += (table[iLeft + 1U] - yL_0d0) * fractions[0U];
  iLeft += stride;
  yL_0d1 = table[iLeft];
  return (((table[iLeft + 1U] - yL_0d1) * fractions[0U] + yL_0d1) - yL_0d0) *
    frac + yL_0d0;
}

int32_T div_nde_s32_floor(int32_T numerator, int32_T denominator)
{
  return (((numerator < 0) != (denominator < 0)) && (numerator % denominator !=
           0) ? -1 : 0) + numerator / denominator;
}

/* Function for MATLAB Function: '<S16>/updatePx' */
static real_T E27_SingleCell_Model_xnrm2_n(int32_T n, const real_T x[27],
  int32_T ix0)
{
  real_T y;
  int32_T k;
  y = 0.0;
  if (n >= 1) {
    if (n == 1) {
      y = fabsf(x[ix0 - 1]);
    } else {
      real_T scale;
      int32_T kend;
      scale = 3.3121686421112381E-170;
      kend = ix0 + n;
      for (k = ix0; k < kend; k++) {
        real_T absxk;
        absxk = fabsf(x[k - 1]);
        if (absxk > scale) {
          real_T t;
          t = scale / absxk;
          y = y * t * t + 1.0;
          scale = absxk;
        } else {
          real_T t;
          t = absxk / scale;
          y += t * t;
        }
      }

      y = scale * sqrtf(y);
    }
  }

  return y;
}

real_T rt_hypotd_snf(real_T u0, real_T u1)
{
  real_T a;
  real_T b;
  real_T y;
  a = fabsf(u0);
  b = fabsf(u1);
  if (a < b) {
    a /= b;
    y = sqrtf(a * a + 1.0) * b;
  } else if (a > b) {
    b /= a;
    y = sqrtf(b * b + 1.0) * a;
  } else if (rtIsNaN(b)) {
    y = (rtNaN);
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

/* Function for MATLAB Function: '<S16>/updatePx' */
static void E27_SingleCell_Model_xgemv(int32_T m, int32_T n, const real_T A[27],
  int32_T ia0, const real_T x[27], int32_T ix0, real_T y[3])
{
  int32_T b_iy;
  int32_T ia;
  if ((m != 0) && (n != 0)) {
    int32_T b;
    if (n - 1 >= 0) {
      memset(&y[0], 0, (uint32_T)n * sizeof(real_T));
    }

    b = (n - 1) * 9 + ia0;
    for (b_iy = ia0; b_iy <= b; b_iy += 9) {
      real_T c;
      int32_T d;
      c = 0.0;
      d = b_iy + m;
      for (ia = b_iy; ia < d; ia++) {
        c += x[((ix0 + ia) - b_iy) - 1] * A[ia - 1];
      }

      ia = div_nde_s32_floor(b_iy - ia0, 9);
      y[ia] += c;
    }
  }
}

/* Function for MATLAB Function: '<S16>/updatePx' */
static void E27_SingleCell_Model_xgerc(int32_T m, int32_T n, real_T alpha1,
  int32_T ix0, const real_T y[3], real_T A[27], int32_T ia0)
{
  int32_T ijA;
  int32_T j;
  if (!(alpha1 == 0.0)) {
    int32_T jA;
    jA = ia0;
    for (j = 0; j < n; j++) {
      real_T temp;
      temp = y[j];
      if (temp != 0.0) {
        int32_T b;
        temp *= alpha1;
        b = m + jA;
        for (ijA = jA; ijA < b; ijA++) {
          A[ijA - 1] += A[((ix0 + ijA) - jA) - 1] * temp;
        }
      }

      jA += 9;
    }
  }
}

/* Function for MATLAB Function: '<S16>/updatePx' */
static void E27_SingleCell_Model_qr(const real_T A[27], real_T b_Q[27], real_T
  R[9])
{
  __m128d tmp;
  real_T b_A[27];
  real_T tau[3];
  real_T work[3];
  real_T atmp;
  real_T b_A_0;
  real_T xnorm;
  int32_T b_coltop;
  int32_T b_lastv;
  int32_T c_k;
  int32_T c_lastc;
  int32_T d_i;
  int32_T d_k;
  int32_T exitg1;
  int32_T ii;
  int32_T knt;
  boolean_T exitg2;
  memcpy(&b_A[0], &A[0], 27U * sizeof(real_T));
  work[0] = 0.0;
  work[1] = 0.0;
  work[2] = 0.0;
  for (d_i = 0; d_i < 3; d_i++) {
    ii = d_i * 9 + d_i;
    atmp = b_A[ii];
    b_coltop = ii + 2;
    tau[d_i] = 0.0;
    xnorm = E27_SingleCell_Model_xnrm2_n(8 - d_i, b_A, ii + 2);
    if (xnorm != 0.0) {
      b_A_0 = b_A[ii];
      xnorm = rt_hypotd_snf(b_A_0, xnorm);
      if (b_A_0 >= 0.0) {
        xnorm = -xnorm;
      }

      if (fabsf(xnorm) < 1.0020841800044864E-292) {
        knt = 0;
        d_k = (ii - d_i) + 9;
        do {
          knt++;
          b_lastv = (((((d_k - ii) - 1) / 2) << 1) + ii) + 2;
          c_lastc = b_lastv - 2;
          for (c_k = b_coltop; c_k <= c_lastc; c_k += 2) {
            tmp = _mm_loadu_pd(&b_A[c_k - 1]);
            _mm_storeu_pd(&b_A[c_k - 1], _mm_mul_pd(tmp, _mm_set1_pd
              (9.9792015476736E+291)));
          }

          for (c_k = b_lastv; c_k <= d_k; c_k++) {
            b_A[c_k - 1] *= 9.9792015476736E+291;
          }

          xnorm *= 9.9792015476736E+291;
          atmp *= 9.9792015476736E+291;
        } while ((fabsf(xnorm) < 1.0020841800044864E-292) && (knt < 20));

        xnorm = rt_hypotd_snf(atmp, E27_SingleCell_Model_xnrm2_n(8 - d_i, b_A,
          ii + 2));
        if (atmp >= 0.0) {
          xnorm = -xnorm;
        }

        tau[d_i] = (xnorm - atmp) / xnorm;
        atmp = 1.0 / (atmp - xnorm);
        c_lastc = b_lastv - 2;
        for (c_k = b_coltop; c_k <= c_lastc; c_k += 2) {
          tmp = _mm_loadu_pd(&b_A[c_k - 1]);
          _mm_storeu_pd(&b_A[c_k - 1], _mm_mul_pd(tmp, _mm_set1_pd(atmp)));
        }

        for (c_k = b_lastv; c_k <= d_k; c_k++) {
          b_A[c_k - 1] *= atmp;
        }

        for (b_lastv = 0; b_lastv < knt; b_lastv++) {
          xnorm *= 1.0020841800044864E-292;
        }

        atmp = xnorm;
      } else {
        tau[d_i] = (xnorm - b_A_0) / xnorm;
        atmp = 1.0 / (b_A_0 - xnorm);
        knt = (ii - d_i) + 9;
        b_lastv = (((((knt - ii) - 1) / 2) << 1) + ii) + 2;
        c_lastc = b_lastv - 2;
        for (d_k = b_coltop; d_k <= c_lastc; d_k += 2) {
          tmp = _mm_loadu_pd(&b_A[d_k - 1]);
          _mm_storeu_pd(&b_A[d_k - 1], _mm_mul_pd(tmp, _mm_set1_pd(atmp)));
        }

        for (d_k = b_lastv; d_k <= knt; d_k++) {
          b_A[d_k - 1] *= atmp;
        }

        atmp = xnorm;
      }
    }

    b_A[ii] = atmp;
    if (d_i + 1 < 3) {
      b_A[ii] = 1.0;
      if (tau[d_i] != 0.0) {
        b_lastv = 9 - d_i;
        c_lastc = (ii - d_i) + 8;
        while ((b_lastv > 0) && (b_A[c_lastc] == 0.0)) {
          b_lastv--;
          c_lastc--;
        }

        c_lastc = 2 - d_i;
        exitg2 = false;
        while ((!exitg2) && (c_lastc > 0)) {
          b_coltop = ((c_lastc - 1) * 9 + ii) + 9;
          knt = b_coltop;
          do {
            exitg1 = 0;
            if (knt + 1 <= b_coltop + b_lastv) {
              if (b_A[knt] != 0.0) {
                exitg1 = 1;
              } else {
                knt++;
              }
            } else {
              c_lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        b_lastv = 0;
        c_lastc = 0;
      }

      if (b_lastv > 0) {
        E27_SingleCell_Model_xgemv(b_lastv, c_lastc, b_A, ii + 10, b_A, ii + 1,
          work);
        E27_SingleCell_Model_xgerc(b_lastv, c_lastc, -tau[d_i], ii + 1, work,
          b_A, ii + 10);
      }

      b_A[ii] = atmp;
    }
  }

  for (d_i = 0; d_i < 3; d_i++) {
    for (ii = 0; ii <= d_i; ii++) {
      R[ii + 3 * d_i] = b_A[9 * d_i + ii];
    }

    for (ii = d_i + 2; ii < 4; ii++) {
      R[(ii + 3 * d_i) - 1] = 0.0;
    }

    work[d_i] = 0.0;
  }

  for (d_i = 2; d_i >= 0; d_i--) {
    ii = d_i * 9 + d_i;
    if (d_i + 1 < 3) {
      b_A[ii] = 1.0;
      if (tau[d_i] != 0.0) {
        b_lastv = 9 - d_i;
        c_lastc = (ii - d_i) + 8;
        while ((b_lastv > 0) && (b_A[c_lastc] == 0.0)) {
          b_lastv--;
          c_lastc--;
        }

        c_lastc = 2 - d_i;
        exitg2 = false;
        while ((!exitg2) && (c_lastc > 0)) {
          b_coltop = ((c_lastc - 1) * 9 + ii) + 9;
          knt = b_coltop;
          do {
            exitg1 = 0;
            if (knt + 1 <= b_coltop + b_lastv) {
              if (b_A[knt] != 0.0) {
                exitg1 = 1;
              } else {
                knt++;
              }
            } else {
              c_lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        b_lastv = 0;
        c_lastc = 0;
      }

      if (b_lastv > 0) {
        E27_SingleCell_Model_xgemv(b_lastv, c_lastc, b_A, ii + 10, b_A, ii + 1,
          work);
        E27_SingleCell_Model_xgerc(b_lastv, c_lastc, -tau[d_i], ii + 1, work,
          b_A, ii + 10);
      }
    }

    b_coltop = (ii - d_i) + 9;
    b_lastv = (((((b_coltop - ii) - 1) / 2) << 1) + ii) + 2;
    c_lastc = b_lastv - 2;
    for (knt = ii + 2; knt <= c_lastc; knt += 2) {
      tmp = _mm_loadu_pd(&b_A[knt - 1]);
      _mm_storeu_pd(&b_A[knt - 1], _mm_mul_pd(tmp, _mm_set1_pd(-tau[d_i])));
    }

    for (knt = b_lastv; knt <= b_coltop; knt++) {
      b_A[knt - 1] *= -tau[d_i];
    }

    b_A[ii] = 1.0 - tau[d_i];
    for (b_lastv = 0; b_lastv < d_i; b_lastv++) {
      b_A[(ii - b_lastv) - 1] = 0.0;
    }
  }

  for (d_i = 0; d_i < 9; d_i++) {
    b_Q[d_i] = b_A[d_i];
    b_Q[d_i + 9] = b_A[d_i + 9];
    b_Q[d_i + 18] = b_A[d_i + 18];
  }
}

/* Function for MATLAB Function: '<S16>/updatePy' */
static void E27_SingleCell_Model_rotate(real_T x, real_T y, real_T *c, real_T *s,
  real_T *r)
{
  real_T absx;
  real_T absy;
  real_T rho;
  real_T xx;
  real_T yy;
  absx = fabsf(x);
  absy = fabsf(y);
  if (absy == 0.0) {
    *c = 1.0;
    *s = 0.0;
    *r = x;
  } else if (absx == 0.0) {
    *c = 0.0;
    *s = 1.0;
    *r = y;
  } else {
    absy += absx;
    xx = x / absy;
    yy = y / absy;
    absx = fabsf(xx);
    rho = rt_hypotd_snf(absx, fabsf(yy));
    *c = absx / rho;
    xx /= absx;
    *s = xx * yy / rho;
    *r = rho * absy * xx;
  }
}

/* Function for MATLAB Function: '<S16>/updatePy' */
static real_T E27_SingleCell_Model_xnrm2(int32_T n, const real_T x[7], int32_T
  ix0)
{
  real_T y;
  int32_T k;
  y = 0.0;
  if (n >= 1) {
    if (n == 1) {
      y = fabsf(x[ix0 - 1]);
    } else {
      real_T scale;
      int32_T kend;
      scale = 3.3121686421112381E-170;
      kend = ix0 + n;
      for (k = ix0; k < kend; k++) {
        real_T absxk;
        absxk = fabsf(x[k - 1]);
        if (absxk > scale) {
          real_T t;
          t = scale / absxk;
          y = y * t * t + 1.0;
          scale = absxk;
        } else {
          real_T t;
          t = absxk / scale;
          y += t * t;
        }
      }

      y = scale * sqrtf(y);
    }
  }

  return y;
}

/* Model step function */
void E27_SingleCell_Model_step(void)
{
  __m128d tmp_1;
  real_T b[49];
  real_T a__1[27];
  real_T residual_1[27];
  real_T residual[21];
  real_T rtb_ImpAsg_InsertedFor_Wm_ixk1_[21];
  real_T rtb_Gain_h[9];
  real_T rtb_MathFunction[9];
  real_T residual_0[7];
  real_T rtb_ImpAsg_InsertedFor_y_at_inp[7];
  real_T rtb_Y1[7];
  real_T b_c[3];
  real_T rtb_Add[3];
  real_T rtb_Kk1_c[3];
  real_T rtb_Product5[3];
  real_T s[3];
  real_T x[3];
  real_T tmp[2];
  real_T tmp_0[2];
  real_T absxk;
  real_T rtb_C1;
  real_T rtb_MathFunction_l;
  real_T rtb_X1_0;
  real_T scale;
  real_T t;
  int32_T ForEach_itr_o;
  int32_T i;
  int32_T iAcol;
  int32_T rtb_MathFunction_tmp;
  boolean_T errorCondition;

  /* Outputs for Iterator SubSystem: '<Root>/SOC Estimator (Adaptive Kalman Filter)' incorporates:
   *  ForEach: '<S1>/For Each'
   */
  /* Delay: '<S15>/Delay' incorporates:
   *  Constant: '<S15>/Constant1'
   *  Inport: '<Root>/Initial R0'
   *  Inport: '<Root>/Initial SoC'
   *  Reshape: '<S15>/Reshape'
   */
  if (E27_SingleCell_Model_DW.CoreSubsys[0].icLoad) {
    E27_SingleCell_Model_DW.CoreSubsys[0].Delay_DSTATE[0] =
      E27_SingleCell_Model_U.temp_p;
    E27_SingleCell_Model_DW.CoreSubsys[0].Delay_DSTATE[1] = 0.0;
    E27_SingleCell_Model_DW.CoreSubsys[0].Delay_DSTATE[2] =
      E27_SingleCell_Model_U.temp_g;
  }

  /* Product: '<S17>/Matrix Multiply' incorporates:
   *  Delay: '<S15>/Delay'
   *  Math: '<S16>/Math Function'
   */
  for (ForEach_itr_o = 0; ForEach_itr_o < 3; ForEach_itr_o++) {
    rtb_MathFunction[3 * ForEach_itr_o] = E27_SingleCell_Model_DW.CoreSubsys[0].
      Delay_DSTATE[0];
    rtb_MathFunction[3 * ForEach_itr_o + 1] =
      E27_SingleCell_Model_DW.CoreSubsys[0].Delay_DSTATE[1];
    rtb_MathFunction[3 * ForEach_itr_o + 2] =
      E27_SingleCell_Model_DW.CoreSubsys[0].Delay_DSTATE[2];
  }

  /* End of Product: '<S17>/Matrix Multiply' */

  /* Gain: '<S17>/Gain' incorporates:
   *  UnitDelay: '<S13>/Unit Delay - P'
   */
  for (i = 0; i < 9; i++) {
    rtb_Gain_h[i] = 1.7320508075688772 * E27_SingleCell_Model_DW.CoreSubsys[0].
      UnitDelayP_DSTATE[i];
  }

  /* End of Gain: '<S17>/Gain' */

  /* Assignment: '<S17>/Assignment2' incorporates:
   *  Assignment: '<S17>/Assignment'
   */
  memset(&E27_SingleCell_Model_B.CoreSubsys[0].Assignment2[0], 0, 21U * sizeof
         (real_T));
  for (ForEach_itr_o = 0; ForEach_itr_o < 3; ForEach_itr_o++) {
    /* Assignment: '<S17>/Assignment' incorporates:
     *  Assignment: '<S17>/Assignment2'
     *  Delay: '<S15>/Delay'
     */
    E27_SingleCell_Model_B.CoreSubsys[0].Assignment2[ForEach_itr_o] =
      E27_SingleCell_Model_DW.CoreSubsys[0].Delay_DSTATE[ForEach_itr_o];

    /* Sum: '<S17>/Sum' incorporates:
     *  Gain: '<S17>/Gain'
     *  Math: '<S16>/Math Function'
     */
    rtb_MathFunction_l = rtb_MathFunction[3 * ForEach_itr_o];
    rtb_C1 = rtb_Gain_h[3 * ForEach_itr_o];

    /* Assignment: '<S17>/Assignment1' incorporates:
     *  Assignment: '<S17>/Assignment2'
     *  Sum: '<S17>/Sum'
     */
    i = (ForEach_itr_o + 1) * 3;
    E27_SingleCell_Model_B.CoreSubsys[0].Assignment2[i] = rtb_MathFunction_l +
      rtb_C1;

    /* Assignment: '<S17>/Assignment2' incorporates:
     *  Sum: '<S17>/Sum1'
     */
    iAcol = (ForEach_itr_o + 4) * 3;
    E27_SingleCell_Model_B.CoreSubsys[0].Assignment2[iAcol] = rtb_MathFunction_l
      - rtb_C1;

    /* Sum: '<S17>/Sum' incorporates:
     *  Gain: '<S17>/Gain'
     *  Math: '<S16>/Math Function'
     */
    rtb_MathFunction_tmp = 3 * ForEach_itr_o + 1;
    rtb_MathFunction_l = rtb_MathFunction[rtb_MathFunction_tmp];
    rtb_C1 = rtb_Gain_h[rtb_MathFunction_tmp];

    /* Assignment: '<S17>/Assignment1' incorporates:
     *  Assignment: '<S17>/Assignment2'
     *  Sum: '<S17>/Sum'
     */
    E27_SingleCell_Model_B.CoreSubsys[0].Assignment2[i + 1] = rtb_MathFunction_l
      + rtb_C1;

    /* Assignment: '<S17>/Assignment2' incorporates:
     *  Sum: '<S17>/Sum1'
     */
    E27_SingleCell_Model_B.CoreSubsys[0].Assignment2[iAcol + 1] =
      rtb_MathFunction_l - rtb_C1;

    /* Sum: '<S17>/Sum' incorporates:
     *  Gain: '<S17>/Gain'
     *  Math: '<S16>/Math Function'
     */
    rtb_MathFunction_tmp = 3 * ForEach_itr_o + 2;
    rtb_MathFunction_l = rtb_MathFunction[rtb_MathFunction_tmp];
    rtb_C1 = rtb_Gain_h[rtb_MathFunction_tmp];

    /* Assignment: '<S17>/Assignment1' incorporates:
     *  Assignment: '<S17>/Assignment2'
     *  Sum: '<S17>/Sum'
     */
    E27_SingleCell_Model_B.CoreSubsys[0].Assignment2[i + 2] = rtb_MathFunction_l
      + rtb_C1;

    /* Assignment: '<S17>/Assignment2' incorporates:
     *  Sum: '<S17>/Sum1'
     */
    E27_SingleCell_Model_B.CoreSubsys[0].Assignment2[iAcol + 2] =
      rtb_MathFunction_l - rtb_C1;
  }

  /* Outputs for Iterator SubSystem: '<S16>/State function' incorporates:
   *  ForEach: '<S22>/For Each'
   */
  for (ForEach_itr_o = 0; ForEach_itr_o < 7; ForEach_itr_o++) {
    /* ForEachSliceSelector generated from: '<S22>/X' incorporates:
     *  Assignment: '<S17>/Assignment2'
     *  Lookup_n-D: '<S28>/C1'
     *  Lookup_n-D: '<S28>/R1'
     */
    scale = E27_SingleCell_Model_B.CoreSubsys[0].Assignment2[3 * ForEach_itr_o];

    /* Lookup_n-D: '<S28>/R1' incorporates:
     *  ForEachSliceSelector generated from: '<S22>/X'
     *  Inport: '<Root>/Measured Temperature'
     */
    rtb_MathFunction_l = look2_pbinlcpw(scale, E27_SingleCell_Model_U.temp,
      E27_SingleCell_Model_ConstP.R1_bp01Data,
      E27_SingleCell_Model_ConstP.R1_bp02Data,
      E27_SingleCell_Model_ConstP.R1_tableData,
      E27_SingleCell_Model_DW.CoreSubsys[0].CoreSubsys[ForEach_itr_o].m_bpIndex,
      E27_SingleCell_Model_ConstP.pooled4, 7U);

    /* Lookup_n-D: '<S28>/C1' incorporates:
     *  Inport: '<Root>/Measured Temperature'
     */
    rtb_C1 = look2_pbinlcpw(scale, E27_SingleCell_Model_U.temp,
      E27_SingleCell_Model_ConstP.C1_bp01Data,
      E27_SingleCell_Model_ConstP.C1_bp02Data,
      E27_SingleCell_Model_ConstP.C1_tableData,
      E27_SingleCell_Model_DW.CoreSubsys[0].CoreSubsys[ForEach_itr_o].
      m_bpIndex_e, E27_SingleCell_Model_ConstP.pooled4, 7U);

    /* Selector: '<S22>/Selector' incorporates:
     *  Constant: '<S22>/Constant'
     */
    t = E27_SingleCell_Model_ConstP.pooled2[ForEach_itr_o];

    /* Sum: '<S28>/Add' incorporates:
     *  Gain: '<S1>/Gain'
     *  Gain: '<S28>/Gain'
     *  Inport: '<Root>/Current'
     *  Product: '<S28>/Product'
     */
    scale += (real32_T)(-1.02880658436214E-5 * -E27_SingleCell_Model_U.Current *
                        E27_SingleCell_Model_B.CoreSubsys[0]
                        .CoreSubsys[ForEach_itr_o].Probe[0]);

    /* ForEachSliceAssignment generated from: '<S22>/Wm_i*xk1_i' incorporates:
     *  Product: '<S22>/Product'
     *  Sum: '<S28>/Add'
     */
    rtb_ImpAsg_InsertedFor_Wm_ixk1_[3 * ForEach_itr_o] = t * scale;

    /* ForEachSliceAssignment generated from: '<S22>/xk1' incorporates:
     *  Sum: '<S28>/Add'
     */
    E27_SingleCell_Model_B.CoreSubsys[0].ImpAsg_InsertedFor_xk1_at_inpor[3 *
      ForEach_itr_o] = scale;

    /* Sum: '<S28>/Add' incorporates:
     *  Assignment: '<S17>/Assignment2'
     *  ForEachSliceSelector generated from: '<S22>/X'
     *  Gain: '<S1>/Gain'
     *  Inport: '<Root>/Current'
     *  Product: '<S28>/Divide1'
     *  Product: '<S28>/Divide2'
     *  Product: '<S28>/Product'
     *  Product: '<S28>/Product1'
     *  Sum: '<S28>/Add1'
     */
    i = 3 * ForEach_itr_o + 1;
    scale = E27_SingleCell_Model_B.CoreSubsys[0].Assignment2[i];
    scale += (real32_T)((-E27_SingleCell_Model_U.Current / rtb_C1 - scale /
                         (rtb_MathFunction_l * rtb_C1)) *
                        E27_SingleCell_Model_B.CoreSubsys[0]
                        .CoreSubsys[ForEach_itr_o].Probe[0]);

    /* ForEachSliceAssignment generated from: '<S22>/Wm_i*xk1_i' incorporates:
     *  Product: '<S22>/Product'
     *  Sum: '<S28>/Add'
     */
    rtb_ImpAsg_InsertedFor_Wm_ixk1_[i] = t * scale;

    /* ForEachSliceAssignment generated from: '<S22>/xk1' incorporates:
     *  Sum: '<S28>/Add'
     */
    E27_SingleCell_Model_B.CoreSubsys[0].ImpAsg_InsertedFor_xk1_at_inpor[i] =
      scale;

    /* ForEachSliceSelector generated from: '<S22>/X' incorporates:
     *  Sum: '<S28>/Add'
     */
    i = 3 * ForEach_itr_o + 2;

    /* Sum: '<S28>/Add' incorporates:
     *  Assignment: '<S17>/Assignment2'
     *  Constant: '<S28>/Constant1'
     *  ForEachSliceSelector generated from: '<S22>/X'
     *  Product: '<S28>/Product'
     */
    scale = 0.0 * E27_SingleCell_Model_B.CoreSubsys[0].CoreSubsys[ForEach_itr_o]
      .Probe[0] + E27_SingleCell_Model_B.CoreSubsys[0].Assignment2[i];

    /* ForEachSliceAssignment generated from: '<S22>/Wm_i*xk1_i' incorporates:
     *  Product: '<S22>/Product'
     *  Sum: '<S28>/Add'
     */
    rtb_ImpAsg_InsertedFor_Wm_ixk1_[i] = t * scale;

    /* ForEachSliceAssignment generated from: '<S22>/xk1' incorporates:
     *  Sum: '<S28>/Add'
     */
    E27_SingleCell_Model_B.CoreSubsys[0].ImpAsg_InsertedFor_xk1_at_inpor[i] =
      scale;
  }

  /* End of Outputs for SubSystem: '<S16>/State function' */

  /* Sum: '<S16>/Sum of Elements' incorporates:
   *  ForEachSliceAssignment generated from: '<S22>/Wm_i*xk1_i'
   *  MATLAB Function: '<S16>/updatePx'
   */
  for (ForEach_itr_o = 0; ForEach_itr_o < 3; ForEach_itr_o++) {
    t = -0.0;
    for (i = 0; i < 7; i++) {
      t += rtb_ImpAsg_InsertedFor_Wm_ixk1_[i * 3 + ForEach_itr_o];
    }

    rtb_Add[ForEach_itr_o] = t;
  }

  scale = rtb_Add[0];
  rtb_MathFunction_l = rtb_Add[1];
  t = rtb_Add[2];

  /* MATLAB Function: '<S16>/updatePx' incorporates:
   *  ForEachSliceAssignment generated from: '<S22>/xk1'
   *  Sum: '<S16>/Sum of Elements'
   */
  for (i = 0; i < 7; i++) {
    rtb_ImpAsg_InsertedFor_Wm_ixk1_[3 * i] = E27_SingleCell_Model_B.CoreSubsys[0]
      .ImpAsg_InsertedFor_xk1_at_inpor[3 * i] - scale;
    ForEach_itr_o = 3 * i + 1;
    rtb_ImpAsg_InsertedFor_Wm_ixk1_[ForEach_itr_o] =
      E27_SingleCell_Model_B.CoreSubsys[0]
      .ImpAsg_InsertedFor_xk1_at_inpor[ForEach_itr_o] - rtb_MathFunction_l;
    ForEach_itr_o = 3 * i + 2;
    rtb_ImpAsg_InsertedFor_Wm_ixk1_[ForEach_itr_o] =
      E27_SingleCell_Model_B.CoreSubsys[0]
      .ImpAsg_InsertedFor_xk1_at_inpor[ForEach_itr_o] - t;
    rtb_Y1[i] = sqrtf(fabsf(E27_SingleCell_Model_ConstP.pooled1[i]));
  }

  memset(&b[0], 0, 49U * sizeof(real_T));
  for (iAcol = 0; iAcol < 7; iAcol++) {
    b[iAcol + 7 * iAcol] = rtb_Y1[iAcol];
    residual[3 * iAcol] = 0.0;
    residual[3 * iAcol + 1] = 0.0;
    residual[3 * iAcol + 2] = 0.0;
  }

  for (ForEach_itr_o = 0; ForEach_itr_o < 7; ForEach_itr_o++) {
    rtb_C1 = residual[3 * ForEach_itr_o];
    iAcol = 3 * ForEach_itr_o + 1;
    rtb_MathFunction_l = residual[iAcol];
    rtb_MathFunction_tmp = 3 * ForEach_itr_o + 2;
    scale = residual[rtb_MathFunction_tmp];
    for (i = 0; i < 7; i++) {
      t = b[7 * ForEach_itr_o + i];
      tmp_1 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&rtb_ImpAsg_InsertedFor_Wm_ixk1_[3 * i]), _mm_set1_pd(t)), _mm_set_pd
                         (rtb_MathFunction_l, rtb_C1));
      _mm_storeu_pd(&tmp_0[0], tmp_1);
      rtb_C1 = tmp_0[0];
      rtb_MathFunction_l = tmp_0[1];
      scale += rtb_ImpAsg_InsertedFor_Wm_ixk1_[3 * i + 2] * t;
    }

    residual[rtb_MathFunction_tmp] = scale;
    residual[iAcol] = rtb_MathFunction_l;
    residual[3 * ForEach_itr_o] = rtb_C1;
  }

  for (ForEach_itr_o = 0; ForEach_itr_o < 3; ForEach_itr_o++) {
    for (i = 0; i < 6; i++) {
      residual_1[i + 9 * ForEach_itr_o] = residual[(i + 1) * 3 + ForEach_itr_o];
    }

    residual_1[9 * ForEach_itr_o + 6] =
      E27_SingleCell_Model_ConstP.updatePx_Q[ForEach_itr_o];
    residual_1[9 * ForEach_itr_o + 7] =
      E27_SingleCell_Model_ConstP.updatePx_Q[ForEach_itr_o + 3];
    residual_1[9 * ForEach_itr_o + 8] =
      E27_SingleCell_Model_ConstP.updatePx_Q[ForEach_itr_o + 6];
  }

  E27_SingleCell_Model_qr(residual_1, a__1, rtb_Gain_h);
  for (iAcol = 0; iAcol < 3; iAcol++) {
    if ((2 - iAcol) - 1 >= 0) {
      memset(&rtb_Gain_h[(iAcol << 2) + 1], 0, (uint32_T)(2 - iAcol) * sizeof
             (real_T));
    }

    b_c[iAcol] = 0.0;
    s[iAcol] = 0.0;
  }

  E27_SingleCell_Model_rotate(rtb_Gain_h[0], residual[0], &b_c[0], &s[0],
    &rtb_Gain_h[0]);
  for (i = 0; i < 2; i++) {
    t = residual[i + 1];
    for (ForEach_itr_o = 0; ForEach_itr_o <= i; ForEach_itr_o++) {
      scale = s[ForEach_itr_o];
      rtb_MathFunction_l = scale * t;
      absxk = b_c[ForEach_itr_o];
      iAcol = (i + 1) * 3 + ForEach_itr_o;
      rtb_C1 = rtb_Gain_h[iAcol];
      t = absxk * t - scale * rtb_C1;
      rtb_Gain_h[iAcol] = absxk * rtb_C1 + rtb_MathFunction_l;
    }

    E27_SingleCell_Model_rotate(rtb_Gain_h[(i + 3 * (i + 1)) + 1], t, &b_c[i + 1],
      &s[i + 1], &rtb_Gain_h[(i + 3 * (i + 1)) + 1]);
  }

  /* Outputs for Iterator SubSystem: '<S16>/Measurement function' incorporates:
   *  ForEach: '<S21>/For Each'
   */
  for (i = 0; i < 7; i++) {
    /* Lookup_n-D: '<S26>/OCV Table' incorporates:
     *  ForEachSliceAssignment generated from: '<S22>/xk1'
     *  ForEachSliceSelector generated from: '<S21>/X'
     *  Inport: '<Root>/Measured Temperature'
     */
    rtb_MathFunction_l = look2_pbinlcpw(E27_SingleCell_Model_B.CoreSubsys[0].
      ImpAsg_InsertedFor_xk1_at_inpor[3 * i], E27_SingleCell_Model_U.temp,
      E27_SingleCell_Model_ConstP.OCVTable_bp01Data,
      E27_SingleCell_Model_ConstP.OCVTable_bp02Data,
      E27_SingleCell_Model_ConstP.OCVTable_tableData,
      E27_SingleCell_Model_DW.CoreSubsys[0].CoreSubsys_h[i].m_bpIndex,
      E27_SingleCell_Model_ConstP.pooled4, 7U);

    /* Sum: '<S26>/Add2' incorporates:
     *  ForEachSliceAssignment generated from: '<S22>/xk1'
     *  ForEachSliceSelector generated from: '<S21>/X'
     *  Gain: '<S1>/Gain'
     *  Inport: '<Root>/Current'
     *  Product: '<S26>/Product2'
     */
    rtb_MathFunction_l = (rtb_MathFunction_l -
                          E27_SingleCell_Model_B.CoreSubsys[0].
                          ImpAsg_InsertedFor_xk1_at_inpor[3 * i + 2] *
                          -E27_SingleCell_Model_U.Current) -
      E27_SingleCell_Model_B.CoreSubsys[0].ImpAsg_InsertedFor_xk1_at_inpor[3 * i
      + 1];

    /* ForEachSliceAssignment generated from: '<S21>/Wm_i*y_i' incorporates:
     *  Constant: '<S21>/Constant'
     *  Product: '<S21>/Product'
     *  Selector: '<S21>/Selector'
     */
    rtb_Y1[i] = E27_SingleCell_Model_ConstP.pooled2[i] * rtb_MathFunction_l;

    /* ForEachSliceAssignment generated from: '<S21>/y' */
    rtb_ImpAsg_InsertedFor_y_at_inp[i] = rtb_MathFunction_l;
  }

  /* End of Outputs for SubSystem: '<S16>/Measurement function' */

  /* Sum: '<S16>/Sum of Elements1' incorporates:
   *  ForEachSliceAssignment generated from: '<S21>/Wm_i*y_i'
   */
  rtb_MathFunction_l = -0.0;
  for (ForEach_itr_o = 0; ForEach_itr_o < 7; ForEach_itr_o++) {
    rtb_MathFunction_l += rtb_Y1[ForEach_itr_o];
  }

  /* MATLAB Function: '<S16>/updatePy' incorporates:
   *  ForEachSliceAssignment generated from: '<S21>/y'
   *  MATLAB Function: '<S16>/updatePx'
   *  Sum: '<S16>/Sum of Elements1'
   */
  for (i = 0; i <= 4; i += 2) {
    tmp_1 = _mm_loadu_pd(&rtb_ImpAsg_InsertedFor_y_at_inp[i]);
    _mm_storeu_pd(&rtb_Y1[i], _mm_sub_pd(tmp_1, _mm_set1_pd(rtb_MathFunction_l)));
    tmp[0] = fabsf(E27_SingleCell_Model_ConstP.pooled1[i]);
    tmp[1] = fabsf(E27_SingleCell_Model_ConstP.pooled1[i + 1]);
    tmp_1 = _mm_loadu_pd(&tmp[0]);
    _mm_storeu_pd(&residual_0[i], _mm_sqrtf_pd(tmp_1));
  }

  for (i = 6; i < 7; i++) {
    rtb_Y1[i] = rtb_ImpAsg_InsertedFor_y_at_inp[i] - rtb_MathFunction_l;
    residual_0[i] = sqrtf(fabsf(E27_SingleCell_Model_ConstP.pooled1[i]));
  }

  memset(&b[0], 0, 49U * sizeof(real_T));
  for (iAcol = 0; iAcol < 7; iAcol++) {
    b[iAcol + 7 * iAcol] = residual_0[iAcol];
    residual_0[iAcol] = 0.0;
  }

  for (ForEach_itr_o = 0; ForEach_itr_o < 7; ForEach_itr_o++) {
    rtb_C1 = residual_0[ForEach_itr_o];
    for (i = 0; i < 7; i++) {
      rtb_C1 += b[7 * ForEach_itr_o + i] * rtb_Y1[i];
    }

    residual_0[ForEach_itr_o] = rtb_C1;
  }

  for (ForEach_itr_o = 0; ForEach_itr_o < 6; ForEach_itr_o++) {
    rtb_ImpAsg_InsertedFor_y_at_inp[ForEach_itr_o] = residual_0[ForEach_itr_o +
      1];
  }

  rtb_ImpAsg_InsertedFor_y_at_inp[6] = 0.1;
  t = rtb_ImpAsg_InsertedFor_y_at_inp[0];
  rtb_C1 = E27_SingleCell_Model_xnrm2(6, rtb_ImpAsg_InsertedFor_y_at_inp, 2);
  if (rtb_C1 != 0.0) {
    rtb_C1 = rt_hypotd_snf(rtb_ImpAsg_InsertedFor_y_at_inp[0], rtb_C1);
    if (rtb_ImpAsg_InsertedFor_y_at_inp[0] >= 0.0) {
      rtb_C1 = -rtb_C1;
    }

    if (fabsf(rtb_C1) < 1.0020841800044864E-292) {
      i = 0;
      do {
        i++;
        for (ForEach_itr_o = 0; ForEach_itr_o <= 4; ForEach_itr_o += 2) {
          tmp_1 = _mm_loadu_pd(&rtb_ImpAsg_InsertedFor_y_at_inp[ForEach_itr_o +
                               1]);
          _mm_storeu_pd(&rtb_ImpAsg_InsertedFor_y_at_inp[ForEach_itr_o + 1],
                        _mm_mul_pd(tmp_1, _mm_set1_pd(9.9792015476736E+291)));
        }

        rtb_C1 *= 9.9792015476736E+291;
        t *= 9.9792015476736E+291;
      } while ((fabsf(rtb_C1) < 1.0020841800044864E-292) && (i < 20));

      rtb_C1 = rt_hypotd_snf(t, E27_SingleCell_Model_xnrm2(6,
        rtb_ImpAsg_InsertedFor_y_at_inp, 2));
      if (t >= 0.0) {
        rtb_C1 = -rtb_C1;
      }

      for (ForEach_itr_o = 0; ForEach_itr_o < i; ForEach_itr_o++) {
        rtb_C1 *= 1.0020841800044864E-292;
      }

      t = rtb_C1;
    } else {
      t = rtb_C1;
    }
  }

  E27_SingleCell_Model_rotate(t, residual_0[0], &scale, &absxk, &rtb_C1);
  for (i = 0; i < 7; i++) {
    /* Math: '<S16>/Math Function' */
    rtb_MathFunction[i] = rtb_Y1[i];

    /* Product: '<S16>/Product5' incorporates:
     *  Constant: '<S16>/Constant'
     */
    scale = 0.0;
    absxk = 0.0;
    rtb_X1_0 = 0.0;
    for (ForEach_itr_o = 0; ForEach_itr_o < 7; ForEach_itr_o++) {
      t = E27_SingleCell_Model_ConstP.Constant_Value[7 * i + ForEach_itr_o];
      tmp_1 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&rtb_ImpAsg_InsertedFor_Wm_ixk1_[3 * ForEach_itr_o]), _mm_set1_pd(t)),
                         _mm_set_pd(absxk, scale));
      _mm_storeu_pd(&tmp_0[0], tmp_1);
      scale = tmp_0[0];
      absxk = tmp_0[1];
      rtb_X1_0 += rtb_ImpAsg_InsertedFor_Wm_ixk1_[3 * ForEach_itr_o + 2] * t;
    }

    residual[3 * i + 2] = rtb_X1_0;
    residual[3 * i + 1] = absxk;
    residual[3 * i] = scale;

    /* End of Product: '<S16>/Product5' */
  }

  for (i = 0; i < 3; i++) {
    /* Product: '<S14>/Product1' incorporates:
     *  MATLAB Function: '<S16>/updatePy'
     *  Product: '<S14>/Product6'
     *  Product: '<S16>/Product1'
     */
    t = 0.0;
    for (ForEach_itr_o = 0; ForEach_itr_o < 7; ForEach_itr_o++) {
      t += residual[3 * ForEach_itr_o + i] * rtb_MathFunction[ForEach_itr_o];
    }

    t = t / rtb_C1 / rtb_C1;
    rtb_Kk1_c[i] = t;

    /* End of Product: '<S14>/Product1' */

    /* Product: '<S14>/Product5' incorporates:
     *  MATLAB Function: '<S16>/updatePy'
     *  Product: '<S14>/Product1'
     */
    rtb_Product5[i] = t * rtb_C1;
  }

  /* MATLAB Function: '<S14>/updatePk' incorporates:
   *  Product: '<S14>/Product5'
   */
  for (i = 0; i < 1; i++) {
    for (iAcol = 0; iAcol < 3; iAcol++) {
      if ((2 - iAcol) - 1 >= 0) {
        memset(&rtb_Gain_h[(iAcol << 2) + 1], 0, (uint32_T)(2 - iAcol) * sizeof
               (real_T));
      }
    }

    errorCondition = (rtb_Gain_h[0] == 0.0);
    if (!errorCondition) {
      errorCondition = (rtb_Gain_h[4] == 0.0);
    }

    if (!errorCondition) {
      errorCondition = (rtb_Gain_h[8] == 0.0);
    }

    if (!errorCondition) {
      x[0] = rtb_Product5[0];
      x[1] = rtb_Product5[1];
      x[2] = rtb_Product5[2];
      rtb_C1 = 0.0;
      scale = 3.3121686421112381E-170;
      for (ForEach_itr_o = 0; ForEach_itr_o < 3; ForEach_itr_o++) {
        iAcol = 3 * ForEach_itr_o;
        t = x[ForEach_itr_o];
        for (rtb_MathFunction_tmp = 0; rtb_MathFunction_tmp < ForEach_itr_o;
             rtb_MathFunction_tmp++) {
          t -= rtb_Gain_h[rtb_MathFunction_tmp + iAcol] * x[rtb_MathFunction_tmp];
        }

        t /= rtb_Gain_h[ForEach_itr_o + iAcol];
        x[ForEach_itr_o] = t;
        absxk = fabsf(t);
        if (absxk > scale) {
          t = scale / absxk;
          rtb_C1 = rtb_C1 * t * t + 1.0;
          scale = absxk;
        } else {
          t = absxk / scale;
          rtb_C1 += t * t;
        }
      }

      rtb_C1 = scale * sqrtf(rtb_C1);
      if (!(rtb_C1 >= 1.0)) {
        rtb_C1 = sqrtf(1.0 - rtb_C1 * rtb_C1);
        scale = fabsf(x[2]);
        if (scale == 0.0) {
          b_c[2] = 1.0;
          s[2] = 0.0;
        } else {
          t = rtb_C1 + scale;
          rtb_C1 /= t;
          scale = x[2] / t;
          absxk = rt_hypotd_snf(rtb_C1, fabsf(scale));
          b_c[2] = rtb_C1 / absxk;
          rtb_C1 /= rtb_C1;
          s[2] = rtb_C1 * scale / absxk;
          rtb_C1 *= absxk * t;
        }

        x[2] = 0.0;
        scale = fabsf(x[1]);
        if (scale == 0.0) {
          b_c[1] = 1.0;
          s[1] = 0.0;
        } else if (rtb_C1 == 0.0) {
          b_c[1] = 0.0;
          s[1] = 1.0;
          rtb_C1 = x[1];
        } else {
          t = rtb_C1 + scale;
          rtb_C1 /= t;
          scale = x[1] / t;
          absxk = rt_hypotd_snf(rtb_C1, fabsf(scale));
          b_c[1] = rtb_C1 / absxk;
          rtb_C1 /= rtb_C1;
          s[1] = rtb_C1 * scale / absxk;
          rtb_C1 *= absxk * t;
        }

        x[1] = 0.0;
        t = fabsf(rtb_C1);
        scale = fabsf(x[0]);
        if (scale == 0.0) {
          b_c[0] = 1.0;
          s[0] = 0.0;
        } else if (t == 0.0) {
          b_c[0] = 0.0;
          s[0] = 1.0;
        } else {
          t += scale;
          rtb_C1 /= t;
          scale = x[0] / t;
          t = fabsf(rtb_C1);
          absxk = rt_hypotd_snf(t, fabsf(scale));
          b_c[0] = t / absxk;
          s[0] = rtb_C1 / t * scale / absxk;
        }

        x[0] = 0.0;
        for (rtb_MathFunction_tmp = 0; rtb_MathFunction_tmp < 3;
             rtb_MathFunction_tmp++) {
          for (ForEach_itr_o = rtb_MathFunction_tmp + 1; ForEach_itr_o >= 1;
               ForEach_itr_o--) {
            scale = s[ForEach_itr_o - 1];
            iAcol = (3 * rtb_MathFunction_tmp + ForEach_itr_o) - 1;
            rtb_C1 = rtb_Gain_h[iAcol];
            absxk = b_c[ForEach_itr_o - 1];
            t = x[rtb_MathFunction_tmp];
            rtb_Gain_h[iAcol] = absxk * rtb_C1 - scale * t;
            x[rtb_MathFunction_tmp] = absxk * t + scale * rtb_C1;
          }
        }
      }
    }
  }

  /* End of MATLAB Function: '<S14>/updatePk' */

  /* Sum: '<S13>/Sum' incorporates:
   *  Inport: '<Root>/CellVoltage'
   *  Sum: '<S16>/Sum of Elements1'
   */
  rtb_MathFunction_l = E27_SingleCell_Model_U.CellVoltage - rtb_MathFunction_l;

  /* Update for Delay: '<S15>/Delay' */
  E27_SingleCell_Model_DW.CoreSubsys[0].icLoad = false;

  /* ForEachSliceAssignment generated from: '<S1>/R0' incorporates:
   *  Product: '<S14>/Product1'
   *  Product: '<S14>/Product3'
   *  Sum: '<S14>/Sum'
   *  Sum: '<S16>/Sum of Elements'
   */
  E27_SingleCell_Model_Y.R0Est = rtb_Kk1_c[0] * rtb_MathFunction_l + rtb_Add[0];

  /* Sum: '<S14>/Sum' incorporates:
   *  Product: '<S14>/Product1'
   *  Product: '<S14>/Product3'
   */
  rtb_Kk1_c[0] = E27_SingleCell_Model_Y.R0Est;

  /* Update for Delay: '<S15>/Delay' incorporates:
   *  Product: '<S14>/Product1'
   *  Product: '<S14>/Product3'
   *  Sum: '<S14>/Sum'
   *  Sum: '<S16>/Sum of Elements'
   * */
  E27_SingleCell_Model_DW.CoreSubsys[0].Delay_DSTATE[0] =
    E27_SingleCell_Model_Y.R0Est;
  E27_SingleCell_Model_DW.CoreSubsys[0].Delay_DSTATE[1] = rtb_Kk1_c[1] *
    rtb_MathFunction_l + rtb_Add[1];

  /* ForEachSliceAssignment generated from: '<S1>/R0' incorporates:
   *  Product: '<S14>/Product1'
   *  Product: '<S14>/Product3'
   *  Sum: '<S14>/Sum'
   *  Sum: '<S16>/Sum of Elements'
   */
  E27_SingleCell_Model_Y.R0Est = rtb_Kk1_c[2] * rtb_MathFunction_l + rtb_Add[2];

  /* Update for Delay: '<S15>/Delay' incorporates:
   *  Sum: '<S14>/Sum'
   */
  E27_SingleCell_Model_DW.CoreSubsys[0].Delay_DSTATE[2] =
    E27_SingleCell_Model_Y.R0Est;

  /* Update for UnitDelay: '<S13>/Unit Delay - P' */
  memcpy(&E27_SingleCell_Model_DW.CoreSubsys[0].UnitDelayP_DSTATE[0],
         &rtb_Gain_h[0], 9U * sizeof(real_T));

  /* ForEachSliceAssignment generated from: '<S1>/SOC' */
  E27_SingleCell_Model_Y.SoCEst = rtb_Kk1_c[0];

  /* End of Outputs for SubSystem: '<Root>/SOC Estimator (Adaptive Kalman Filter)' */
}

/* Model initialize function */
void E27_SingleCell_Model_initialize(void)
{
    rtNaN = -(real_T)NAN;
    rtNaNF = -(real32_T)NAN;

  {
    int32_T ForEach_itr_o;

    /* SystemInitialize for Iterator SubSystem: '<Root>/SOC Estimator (Adaptive Kalman Filter)' */
    /* InitializeConditions for Delay: '<S15>/Delay' */
    E27_SingleCell_Model_DW.CoreSubsys[0].icLoad = true;

    /* InitializeConditions for UnitDelay: '<S13>/Unit Delay - P' */
    memcpy(&E27_SingleCell_Model_DW.CoreSubsys[0].UnitDelayP_DSTATE[0],
           &E27_SingleCell_Model_ConstP.UnitDelayP_InitialCondition[0], 9U *
           sizeof(real_T));

    /* SystemInitialize for Iterator SubSystem: '<S16>/State function' */
    for (ForEach_itr_o = 0; ForEach_itr_o < 7; ForEach_itr_o++) {
      /* Start for Probe: '<S28>/Probe' */
      E27_SingleCell_Model_B.CoreSubsys[0].CoreSubsys[ForEach_itr_o].Probe[0] =
        1.0;
      E27_SingleCell_Model_B.CoreSubsys[0].CoreSubsys[ForEach_itr_o].Probe[1] =
        0.0;
    }

    /* End of SystemInitialize for SubSystem: '<S16>/State function' */
    /* End of SystemInitialize for SubSystem: '<Root>/SOC Estimator (Adaptive Kalman Filter)' */
  }
}

/* Model terminate function */
void E27_SingleCell_Model_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

/* ==========================================================================
 * END MERGED SECTION: E27_SingleCell_Model.c
 * ========================================================================== */


/* ========================================================================== 
 * Convenience API for firmware/application code
 * ========================================================================== */

/*
 * One-call wrapper around the generated global-input/global-output API.
 *
 * The InitialSoC and InitialR0 inputs are only consumed by the generated model
 * while its internal icLoad flag is true, which is after initialize() and before
 * the first step. After that first step, the estimator state is carried forward
 * internally through E27_SingleCell_Model_DW.
 */
void E27_SingleCell_Model_step_with_inputs(real_T current_A,
                                           real_T cell_voltage_V,
                                           real_T measured_temperature_K,
                                           real_T initial_soc_fraction,
                                           real_T initial_r0_ohm,
                                           real_T *soc_est_fraction,
                                           real_T *r0_est_ohm)
{
  E27_SingleCell_Model_U.Current = current_A;
  E27_SingleCell_Model_U.CellVoltage = cell_voltage_V;
  E27_SingleCell_Model_U.temp = measured_temperature_K;
  E27_SingleCell_Model_U.temp_p = initial_soc_fraction;
  E27_SingleCell_Model_U.temp_g = initial_r0_ohm;

  E27_SingleCell_Model_step();

  if (soc_est_fraction != NULL) {
    *soc_est_fraction = E27_SingleCell_Model_Y.SoCEst;
  }

  if (r0_est_ohm != NULL) {
    *r0_est_ohm = E27_SingleCell_Model_Y.R0Est;
  }
}

/*
 * Optional example main.
 * Compile with -DE27_SINGLE_FILE_EXAMPLE_MAIN if you want a quick host-side
 * smoke test executable. Leave the macro undefined for normal firmware builds.
 */
#ifdef E27_SINGLE_FILE_EXAMPLE_MAIN
int main(void)
{
  real_T soc = 0.0;
  real_T r0 = 0.0;

  E27_SingleCell_Model_initialize();

  E27_SingleCell_Model_step_with_inputs(
      0.0,      /* current_A */
      3.70,     /* cell_voltage_V */
      293.0,    /* measured_temperature_K */
      0.50,     /* initial_soc_fraction */
      0.003,    /* initial_r0_ohm */
      &soc,
      &r0);

  printf("SoC estimate = %.9f\nR0 estimate  = %.9f ohm\n", soc, r0);

  E27_SingleCell_Model_terminate();
  return 0;
}
#endif