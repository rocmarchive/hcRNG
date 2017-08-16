#ifndef MTGP32_H_
#define MTGP32_H_

#pragma once

#include "hcRNG/private/mtgp32-fast.h"
#include "hcRNG.h"
// Opaque structure declarations

typedef struct mtgp32_kernel_params mtgp32_kernel_params_t;

typedef struct hcrngStateMtgp32 hcrngStateMtgp32_t;

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

#endif // HCC

