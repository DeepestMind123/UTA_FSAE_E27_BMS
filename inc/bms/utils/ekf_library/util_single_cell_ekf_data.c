/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: E27_SingleCell_Model_data.c
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


/* Constant parameters (default storage) */
const ConstP_E27_SingleCell_Model_T E27_SingleCell_Model_ConstP = {
  /* Pooled Parameter (Expression: Wc)
   * Referenced by:
   *   '<S16>/updatePx'
   *   '<S16>/updatePy'
   */
  { 2.0, 0.16666666666666666, 0.16666666666666666, 0.16666666666666666,
    0.16666666666666666, 0.16666666666666666, 0.16666666666666666 },

  /* Expression: Q
   * Referenced by: '<S16>/updatePx'
   */
  { 1.0E-6, 0.0, 0.0, 0.0, 1.0E-6, 0.0, 0.0, 0.0, 1.0E-6 },

  /* Pooled Parameter (Expression: Wm)
   * Referenced by:
   *   '<S21>/Constant'
   *   '<S22>/Constant'
   */
  { 0.0, 0.16666666666666666, 0.16666666666666666, 0.16666666666666666,
    0.16666666666666666, 0.16666666666666666, 0.16666666666666666 },

  /* Expression: R1_mat
   * Referenced by: '<S28>/R1'
   */
  { 0.0109, 0.0069, 0.0047, 0.0034, 0.0033, 0.0033, 0.0028, 0.0029, 0.0024,
    0.0026, 0.0016, 0.0023, 0.0018, 0.0017, 0.0013, 0.0012, 0.0013, 0.001,
    0.0014, 0.0011, 0.0011 },

  /* Expression: SOC_vec
   * Referenced by: '<S28>/R1'
   */
  { 0.0, 0.1, 0.25, 0.5, 0.75, 0.9, 1.0 },

  /* Expression: T_vec
   * Referenced by: '<S28>/R1'
   */
  { 278.0, 293.0, 313.0 },

  /* Expression: C1_mat
   * Referenced by: '<S28>/C1'
   */
  { 1834.8623853211009, 4492.753623188406, 23191.489361702126,
    10588.235294117647, 17878.78787878788, 12121.212121212122, 8928.57142857143,
    12413.793103448277, 18750.0, 40384.615384615383, 18125.0, 33478.260869565216,
    18333.333333333332, 22941.176470588238, 30000.0, 32500.000000000004,
    46923.076923076922, 26000.0, 47857.142857142855, 26363.63636363636, 30000.0
  },

  /* Expression: SOC_vec
   * Referenced by: '<S28>/C1'
   */
  { 0.0, 0.1, 0.25, 0.5, 0.75, 0.9, 1.0 },

  /* Expression: T_vec
   * Referenced by: '<S28>/C1'
   */
  { 278.0, 293.0, 313.0 },

  /* Expression: V0_mat
   * Referenced by: '<S26>/OCV Table'
   */
  { 3.49, 3.55, 3.62, 3.71, 3.91, 4.07, 4.19, 3.5, 3.57, 3.63, 3.71, 3.93, 4.08,
    4.19, 3.51, 3.56, 3.64, 3.72, 3.94, 4.08, 4.19 },

  /* Expression: SOC_vec
   * Referenced by: '<S26>/OCV Table'
   */
  { 0.0, 0.1, 0.25, 0.5, 0.75, 0.9, 1.0 },

  /* Expression: T_vec
   * Referenced by: '<S26>/OCV Table'
   */
  { 278.0, 293.0, 313.0 },

  /* Expression: diag(Wc)
   * Referenced by: '<S16>/Constant'
   */
  { 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.16666666666666666, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.16666666666666666, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.16666666666666666, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.16666666666666666,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.16666666666666666, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.16666666666666666 },

  /* Expression: chol(P0,'lower')
   * Referenced by: '<S13>/Unit Delay - P'
   */
  { 0.0031622776601683794, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
    0.0031622776601683794 },

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<S26>/OCV Table'
   *   '<S28>/C1'
   *   '<S28>/R1'
   */
  { 6U, 2U }
};