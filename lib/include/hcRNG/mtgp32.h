#ifndef MTGP32_H_
#define MTGP32_H_

#pragma once

#include "hcRNG/private/mtgp32-fast.h"
#include "hcRNG.h"
#define MTGP32_MEXP 11213
#define MTGP32_N 351
#define MTGP32_FLOOR_2P 256
#define MTGP32_CEIL_2P 512
#define MTGP32_TN MTGP32_FLOOR_2P // TBL_NUMBER, do not exceed 256, i.e. WAVEFRONT_SIZE
#define MTGP32_LS (MTGP32_TN * 3)
#define MTGP32_TS 16              // TBL_SIZE
#define HcRAND_GROUP_NUM 200     // < MTGP32_TN, user specifies USER_GROUP_NUM
#define MTGP32_STATE_SIZE 1024    // The effective state number is =351
#define MTGP32_STATE_MASK 1023
#define USER_GROUP_NUM 64         // < HcRAND_GROUP_NUM

#ifndef DIVUP
#define DIVUP(x, y) (((x) + (y) - 1) / (y))
#endif
#define BLOCK_SIZE 256
#define MAX_NUM_BLOCKS 64


// mtgp32_kernel_params
typedef struct mtgp32_kernel_params {
  uint32_t offset[USER_GROUP_NUM]; // size: USER_GROUP_NUM
  uint32_t index[USER_GROUP_NUM];  // size: USER_GROUP_NUM
  uint32_t d_status[USER_GROUP_NUM * MTGP32_STATE_SIZE]; // extent<2>(USER_GROUP_NUM, MTGP32_STATE_SIZE)
  // mtgp32 kernel params
  uint32_t mexp_tbl[HcRAND_GROUP_NUM];                      // size: 1. Redundant
  uint32_t param_tbl[HcRAND_GROUP_NUM * MTGP32_TN];         // extent<2>(HcRAND_GROUP_NUM, MTGP32_TN)
  uint32_t temper_tbl[HcRAND_GROUP_NUM * MTGP32_TN];        // extent<2>(HcRAND_GROUP_NUM, MTGP32_TN)
  uint32_t single_temper_tbl[HcRAND_GROUP_NUM * MTGP32_TN]; // extent<2>(HcRAND_GROUP_NUM, MTGP32_TN)
  uint32_t pos_tbl[MTGP32_TN];  // size: MTGP32_TN
  uint32_t sh1_tbl[MTGP32_TN];  // size: MTGP32_TN
  uint32_t sh2_tbl[MTGP32_TN];  // size: MTGP32_TN
  uint32_t mask[1];             // size: 1
} mtgp32_kernel_params_t;


// Structure of hcrngStateMtgp32
typedef struct hcrngStateMtgp32 {
  uint32_t s[MTGP32_STATE_SIZE];
  uint32_t offset;
  uint32_t pIdx;
  mtgp32_kernel_params* k;  // points to all kernel parameters
} hcrngStateMtgp32_t;


typedef hcrngStatus_ hcrngStatus_t;

namespace hc {
class accelerator;
class accelerator_view;
};

#define __device__ __attribute__((hc))
#define __host__ __attribute__((cpu))

//  Set up initial states for the mtgp32 generator. 
__host__ hcrngStatus_t hcrngMakeMTGP32KernelState (hc::accelerator_view accl_view, hcrngStateMtgp32_t* s, mtgp32_params_fast_t params[], mtgp32_kernel_params_t* k, int  n, unsigned long long seed );

//     Set up constant parameters for the mtgp32 generator. 
__host__ hcrngStatus_t hcrngMakeMTGP32Constants ( hc::accelerator_view accl_view, const mtgp32_params_fast_t params[], mtgp32_kernel_params_t* p );


//  Return a log-normally distributed float from an MTGP32 generator. 
__device__ float hcrng_log_normal ( hcrngStateMtgp32_t* state, float  mean, float  stddev);

//  Return a log-normally distributed double from an MTGP32 generator. 
__device__ double hcrng_log_normal_double ( hcrngStateMtgp32_t* state, double  mean, double  stddev );


//   Return a normally distributed float from a MTGP32 generator. 
__device__ float hcrng_normal ( hcrngStateMtgp32_t* state );

//  Return a normally distributed double from an MTGP32 generator. 
__device__ double hcrng_normal_double ( hcrngStateMtgp32_t* state);


//   Return a uniformly distributed float from a MTGP32 generator. 
__device__ float hcrng_uniform ( hcrngStateMtgp32_t* state );

//  Return a uniformly distributed double from an MTGP32 generator. 
__device__ double hcrng_uniform_double ( hcrngStateMtgp32_t* state);


#endif // MTGP32_H_


