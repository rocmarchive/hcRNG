#ifndef MTGP32_IMPL_H_
#define MTGP32_IMPL_H_

#pragma once

// FIXME: Better to inline every amp-restricted functions and make it static
#include "hcRNG/private/mtgp32-fast.h"
#include "hcRNG/mtgp32.h"
#include <hc.hpp>
#include <hc_defines.h>


void hcrngStateMtgp32_init(
    hc::accelerator_view accl_view, hcrngStateMtgp32* s);
void hcrngStateMtgp32_release(hcrngStateMtgp32* s);
void hcrngStateMtgp32_copy_D2H(void* src, void* dst);
void hcrngStateMtgp32_copy_H2D(void* src, void* dst);

// Device APIs
int mtgp32_init_params_kernel(
    hc::accelerator_view accl_view,
    const mtgp32_params_fast_t* params,
    mtgp32_kernel_params*& s);
int mtgp32_init_seed_kernel(
    hc::accelerator_view accl_view,
    hcrngStateMtgp32* state,
    mtgp32_kernel_params* p,
    int n,
    unsigned long seed);
/**
 * The function of the recursion formula calculation.
 *
 * @param[in] X1 the farthest part of state array.
 * @param[in] X2 the second farthest part of state array.
 * @param[in] Y a part of state array.
 * @return output
 */
static
inline
uint32_t para_rec(
    uint32_t* param_tbl,
    uint32_t sh1,
    uint32_t sh2,
    uint32_t X1,
    uint32_t X2,
    uint32_t Y,
    uint32_t mask) [[cpu]][[hc]]
{
  uint32_t X = (X1 & mask) ^ X2;
  uint32_t MAT;

  X ^= X << sh1;
  Y = X ^ (Y >> sh2);
  MAT = param_tbl[Y & 0x0f];
  return Y ^ MAT;
}

/**
 * The tempering function.
 *
 * @param[in] mtgp mtgp32 structure
 * @param[in] V the output value should be tempered.
 * @param[in] T the tempering helper value.
 * @return the tempered value.
 */
static
inline
uint32_t temper(uint32_t* temper_tbl, uint32_t V, uint32_t T) [[cpu]][[hc]]
{
  uint32_t MAT;

  T ^= T >> 16;
  T ^= T >> 8;
  MAT = temper_tbl[T & 0x0f];
  return V ^ MAT;
}

#define HcRAND_2POW32_INV (2.3283064e-10f)
#define HcRAND_SQRT2 (-1.4142135f)
#define HcRAND_SQRT2_DOUBLE (-1.4142135623730951)

// in one workgroup
inline static
unsigned int hcrng(
    hcrngStateMtgp32 *state) [[hc]]
{  

  const uint32_t* av_param_tbl = (state[0].k->param_tbl); 
  const uint32_t* av_temper_tbl = (state[0].k->temper_tbl); 
  const uint32_t* av_sh1_tbl = (state[0].k->sh1_tbl); 
  const uint32_t* av_sh2_tbl = (state[0].k->sh2_tbl); 
  uint32_t* av_offset = (state[0].k->offset); 
  const uint32_t* av_index = (state[0].k->index); 
  const uint32_t* av_pos_tbl = (state[0].k->pos_tbl); 
  const uint32_t* av_mask = (state[0].k->mask); 
  const uint32_t* av_d_status = (state[0].k->d_status); 
   
  unsigned int t = (hc_get_group_size(2) * hc_get_group_size(1) * hc_get_workitem_id(2)) + (hc_get_group_size(0) * hc_get_workitem_id(1)) + hc_get_workitem_id(0);
  unsigned int d = hc_get_group_size(2) * hc_get_group_size(1) * hc_get_group_size(0) ;
  uint32_t* pParam_tbl = const_cast<uint32_t*>(&av_param_tbl[0]);
  uint32_t* pTemper_tbl = const_cast<uint32_t*>(&av_temper_tbl[0]);
  uint32_t* d_status = const_cast<uint32_t*>(&av_d_status[0]);
  uint32_t offset = av_offset[0];
  uint32_t sh1 = av_sh1_tbl[0];
  uint32_t sh2 = av_sh2_tbl[0];
  int index = av_index[0];
  int pos = av_pos_tbl[index];
  unsigned int r;
  unsigned int o;
  r = para_rec(pParam_tbl, sh1, sh2, d_status[(t + offset) & MTGP32_STATE_MASK],
           d_status[(t + offset + 1) & MTGP32_STATE_MASK],
           d_status[(t + offset + pos) & MTGP32_STATE_MASK],
           av_mask[0]);
  d_status[(t + offset + MTGP32_N) & MTGP32_STATE_MASK] = r;
  o = temper(pTemper_tbl, r, d_status[(t + offset + pos -1) & MTGP32_STATE_MASK]);

  // Thread Synchronization
  hc_barrier(CLK_LOCAL_MEM_FENCE);

  if (t == 0) {
    av_offset[0] = (av_offset[0] + d) & MTGP32_STATE_MASK;
  }

  // Thread Synchronization
  hc_barrier(CLK_LOCAL_MEM_FENCE);
  return o;
}

inline static
float _hcrng_uniform_func(unsigned int x) [[cpu]][[hc]]
{
  return x * HcRAND_2POW32_INV + (HcRAND_2POW32_INV/2.0f);
}


inline float hcrng_uniform_func(
    hcrngStateMtgp32 *state) [[hc]]
{
  unsigned int x = hcrng(
      state);
  return _hcrng_uniform_func(x);
}

inline static
double do_erfcinv(float x, bool refine) [[cpu]][[hc]]
{
  // Coefficients of rational approximation.
  const double a[] =
    { -2.806989788730439e+01,  1.562324844726888e+02,
      -1.951109208597547e+02,  9.783370457507161e+01,
      -2.168328665628878e+01,  1.772453852905383e+00 };
  const double b[] =
    { -5.447609879822406e+01,  1.615858368580409e+02,
      -1.556989798598866e+02,  6.680131188771972e+01,
      -1.328068155288572e+01 };
  const double c[] =
    { -5.504751339936943e-03, -2.279687217114118e-01,
      -1.697592457770869e+00, -1.802933168781950e+00,
       3.093354679843505e+00,  2.077595676404383e+00 };
  const double d[] =
    {  7.784695709041462e-03,  3.224671290700398e-01,
       2.445134137142996e+00,  3.754408661907416e+00 };

  static const double spi2 =  8.862269254527579e-01; // sqrt(pi)/2.
  //static const double pi = 3.14159265358979323846;
  static const double pbreak = 0.95150;
  double y;

  // Select case.
  if (x <= 1+pbreak && x >= 1-pbreak) {
    // Middle region.
    const double q = 0.5*(1-x), r = q*q;
    const double yn =
        (((((a[0]*r + a[1])*r + a[2])*r + a[3])*r + a[4])*r + a[5])*q;
    const double yd =
        ((((b[0]*r + b[1])*r + b[2])*r + b[3])*r + b[4])*r + 1.0;
    y = yn / yd;
  } else if (x < 2.0 && x > 0.0) {
    // Tail region.
    const double q =
        x < 1 ? hc::precise_math::sqrt(-2*hc::precise_math::log((double)0.5*x))
              : hc::precise_math::sqrt(-2*hc::precise_math::log((double)0.5*(2-x)));
    const double yn = ((((c[0]*q + c[1])*q + c[2])*q + c[3])*q + c[4])*q + c[5];
    const double yd = (((d[0]*q + d[1])*q + d[2])*q + d[3])*q + 1.0;
    y = yn / yd;
    if (x < 1-pbreak)
      y *= -1;
  } else if (x == 0.0) {
    return 0; //octave_Inf;
  } else if (x == 2.0) {
    return 0; //-octave_Inf;
  } else {
    return 1; //octave_NaN;
  }

  if (refine) {
    // One iteration of Halley's method gives full precision.
    double u = (erf(y) - (1-x)) * spi2 * exp (y*y);
    y -= u / (1 + y*u);
  }

 return y;
}


inline static
float my_erfcinvf(float x) [[cpu]][[hc]]
{
  return do_erfcinv (x, false);
}

inline static

float _hcrng_normal_func_icdf(unsigned int x) [[cpu]][[hc]]
{
  float s = HcRAND_SQRT2;
  // Mirror to avoid loss of precision
  if (x > 0x80000000UL) {
    x = 0xffffffffUL - x;
    s = -s;
  }
  float p = x * HcRAND_2POW32_INV + (HcRAND_2POW32_INV/2.0f);
  // p is in (0, 0.5], 2p is in (0, 1]
  return s * my_erfcinvf(2.0f * p);
}

inline static
float hcrng_normal_func(
    hcrngStateMtgp32 *state) [[hc]]
{
  unsigned int x = hcrng(
      state);
  return _hcrng_normal_func_icdf(x);
}

inline static
double hcrng_log_normal_func(
    hcrngStateMtgp32 *state,
    double mean,
    double stddev) [[hc]]
{
  unsigned int x = hcrng(
      state);
  return hc::precise_math::exp(mean + ((double)stddev * _hcrng_normal_func_icdf(x)));
}

// User defined wrappers
template<typename T>
inline static
void user_log_normal_kernel(
    hc::accelerator_view accl_view,
    hcrngStateMtgp32 *s,
    T* &av_result,
    double mean,
    double stddev,
    uint32_t size)
{
  int rounded_size = DIVUP(size, BLOCK_SIZE) * BLOCK_SIZE;
  int blocks = std::min((int)DIVUP(size, BLOCK_SIZE), MAX_NUM_BLOCKS);
  hc::extent<1> ext(blocks*BLOCK_SIZE);
  hc::tiled_extent<1> t_ext = ext.tile(BLOCK_SIZE);

  hc::parallel_for_each(
      accl_view, t_ext, [=] (hc::tiled_index<1>& tidx) [[hc]] {
    uint32_t threadId = tidx.global[0];
    uint32_t groupId = tidx.tile[0];
    if (groupId >= USER_GROUP_NUM)
      return;
   for (int i = threadId; i < rounded_size; i += BLOCK_SIZE * MAX_NUM_BLOCKS) {
      double x = hcrng_log_normal_func(
          s,
          mean,
          stddev);
      if (i < size) {
        av_result[i] = x;
      }
    }
  }).wait();
}

// User defined wrappers
template <typename UnaryFunction, typename T>
inline void user_uniform_kernel(
    hc::accelerator_view accl_view,
    hcrngStateMtgp32 *s,
    T* &av_result,
    uint32_t size,
    UnaryFunction f)
{
  int rounded_size = DIVUP(size, BLOCK_SIZE) * BLOCK_SIZE;
  int blocks = std::min((int)DIVUP(size, BLOCK_SIZE), MAX_NUM_BLOCKS);
  hc::extent<1> ext(blocks*BLOCK_SIZE);
  hc::tiled_extent<1> t_ext = ext.tile(BLOCK_SIZE);
  hc::parallel_for_each(
      accl_view, t_ext, [=] (hc::tiled_index<1>& tidx) [[hc]] {
    int threadId = tidx.global[0];
    int groupId = tidx.tile[0];
    if (groupId >= USER_GROUP_NUM)
      return;

    for (int i = threadId; i < rounded_size; i += BLOCK_SIZE * MAX_NUM_BLOCKS) {
      float x = hcrng_uniform_func(
          s);
      if (i < size) {
        double y = f(x);
        av_result[i] = y;
      }
    }
  }).wait();
}


template <typename UnaryFunction, typename T>
inline void user_normal_kernel(
    hc::accelerator_view accl_view,
    hcrngStateMtgp32 *s,
    T* &av_result,
    uint32_t size,
    UnaryFunction f)
{
  int rounded_size = DIVUP(size, BLOCK_SIZE) * BLOCK_SIZE;
  int blocks = std::min((int)DIVUP(size, BLOCK_SIZE), MAX_NUM_BLOCKS);
  hc::extent<1> ext(blocks*BLOCK_SIZE);
  hc::tiled_extent<1> t_ext = ext.tile(BLOCK_SIZE);
  hc::parallel_for_each(
      accl_view, t_ext, [=] (hc::tiled_index<1>& tidx) [[hc]] {
    uint32_t threadId = tidx.global[0];
    uint32_t groupId = tidx.tile[0];
    if (groupId >= USER_GROUP_NUM)
      return;

    for (int i = threadId; i < rounded_size; i += BLOCK_SIZE * MAX_NUM_BLOCKS) {
      float x = hcrng_normal_func(s);
      if (i < size) {
        double y = f(x);
        av_result[i] = y;
      }
    }
  }).wait();
}



#endif // MTGP32_IMPL_H_
