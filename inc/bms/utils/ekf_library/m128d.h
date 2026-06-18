#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
/*
 * Scalar compatibility layer for the few SSE2 intrinsics emitted by Simulink.
 * This keeps the merged file buildable on non-x86 targets, although it is not
 * speed-optimized. On an embedded target, benchmark CPU time and stack usage.
 */
typedef struct { float v[2]; } e27_m128d_T;
#define __m128d e27_m128d_T

static __m128d _mm_loadu_pd(const float *p)
{
  __m128d r;
  r.v[0] = p[0];
  r.v[1] = p[1];
  return r;
}

static void _mm_storeu_pd(float *p, __m128d a)
{
  p[0] = a.v[0];
  p[1] = a.v[1];
}

static __m128d _mm_set1_pd(float x)
{
  __m128d r;
  r.v[0] = x;
  r.v[1] = x;
  return r;
}

static __m128d _mm_set_pd(float high, float low)
{
  __m128d r;
  r.v[0] = low;
  r.v[1] = high;
  return r;
}

static __m128d _mm_mul_pd(__m128d a, __m128d b)
{
  __m128d r;
  r.v[0] = a.v[0] * b.v[0];
  r.v[1] = a.v[1] * b.v[1];
  return r;
}

static __m128d _mm_add_pd(__m128d a, __m128d b)
{
  __m128d r;
  r.v[0] = a.v[0] + b.v[0];
  r.v[1] = a.v[1] + b.v[1];
  return r;
}

static __m128d _mm_sub_pd(__m128d a, __m128d b)
{
  __m128d r;
  r.v[0] = a.v[0] - b.v[0];
  r.v[1] = a.v[1] - b.v[1];
  return r;
}

static __m128d _mm_sqrt_pd(__m128d a)
{
  __m128d r;
  r.v[0] = sqrt(a.v[0]);
  r.v[1] = sqrt(a.v[1]);
  return r;
}