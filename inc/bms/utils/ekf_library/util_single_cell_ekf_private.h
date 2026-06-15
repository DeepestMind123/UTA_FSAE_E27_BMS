/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: E27_SingleCell_Model_private.h
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

#ifndef E27_SingleCell_Model_private_h_
#define E27_SingleCell_Model_private_h_

#include "rtwtypes.h"

extern real_T rt_hypotd_snf(real_T u0, real_T u1);
extern real_T look2_pbinlcpw(real_T u0, real_T u1, const real_T bp0[], const
  real_T bp1[], const real_T table[], uint32_T prevIndex[], const uint32_T
  maxIndex[], uint32_T stride);
extern int32_T div_nde_s32_floor(int32_T numerator, int32_T denominator);

#endif                                 /* E27_SingleCell_Model_private_h_ */
