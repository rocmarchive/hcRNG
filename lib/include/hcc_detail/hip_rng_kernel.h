/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#pragma once

#include <hip/hip_runtime_api.h>
#include <hiprng.h>
#include <hcRNG/mtgp32.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef hcrngStateMtgp32 hiprngStateMtgp32_t;
typedef hcrngStateMtgp32 hiprngStateMtgp32;

typedef mtgp32_kernel_params mtgp32_kernel_params_t;


// Forward declarations
//enum hcrngStatus: short;
typedef void *hiprngGenerator_t;


//  Set up initial states for the mtgp32 generator. 
__host__ hiprngStatus_t hiprngMakeMTGP32KernelState (hiprngStateMtgp32_t* s, mtgp32_params_fast_t params[], mtgp32_kernel_params_t* k, int  n, unsigned long long seed );

//     Set up constant parameters for the mtgp32 generator. 
__host__ hiprngStatus_t hiprngMakeMTGP32Constants ( const mtgp32_params_fast_t params[], mtgp32_kernel_params_t* p );


//  Return a log-normally distributed float from an MTGP32 generator. 
__device__ float hiprng_log_normal ( hiprngStateMtgp32_t* state, float  mean, float  stddev) {
    return hcrng_log_normal(reinterpret_cast<hcrngStateMtgp32*>(state), mean, stddev);
}

//  Return a log-normally distributed double from an MTGP32 generator. 
__device__ double hiprng_log_normal_double ( hiprngStateMtgp32_t* state, double  mean, double  stddev ) {
    return hcrng_log_normal(reinterpret_cast<hcrngStateMtgp32*>(state), mean, stddev);
}


//   Return a normally distributed float from a MTGP32 generator. 
__device__ float hiprng_normal ( hiprngStateMtgp32_t* state ) {
    return hcrng_normal(reinterpret_cast<hcrngStateMtgp32*>(state));
}

//  Return a normally distributed double from an MTGP32 generator. 
__device__ double hiprng_normal_double ( hiprngStateMtgp32_t* state) {
    return hcrng_normal(reinterpret_cast<hcrngStateMtgp32*>(state));
}


//   Return a uniformly distributed float from a MTGP32 generator. 
__device__ float hiprng_uniform ( hiprngStateMtgp32_t* state ) {
    return hcrng_uniform(reinterpret_cast<hcrngStateMtgp32*>(state));
}

//  Return a uniformly distributed double from an MTGP32 generator. 
__device__ double hiprng_uniform_double ( hiprngStateMtgp32_t* state) {
    return hcrng_uniform(reinterpret_cast<hcrngStateMtgp32*>(state));
}


}
