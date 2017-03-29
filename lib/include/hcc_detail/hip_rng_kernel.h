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

#include <hcRNG/hcRNG.h>
#include <hcRNG/mrg32k3a.h>
#include <hcRNG/philox432.h>

#if !defined(QUALIFIERS)
#define QUALIFIERS static __inline__ __device__
#endif

/*#ifdef __cplusplus
extern "C" {
#endif*/

typedef hcrngState_t hiprngState_t;  
typedef hcrngStateMRG32k3a_t hiprngStateMRG32k3a_t;
typedef hcrngStatePhilox4_32_10_t hiprngStatePhilox4_32_10_t;
typedef void *hiprngStateXORWOW_t;

/*QUALIFIERS void hiprng_init ( unsigned long long seed, unsigned long long subsequence, unsigned long long offset, hiprngStateMRG32k3a_t* state ){
  return curand_init(seed, subsequence, offset, state);
}

QUALIFIERS void hiprng_init ( unsigned long long seed, unsigned long long subsequence, unsigned long long offset, hiprngStatePhilox4_32_10_t* state ){
  return curand_init(seed, subsequence, offset, state);
}

QUALIFIERS void hiprng_init ( unsigned long long seed, unsigned long long subsequence, unsigned long long offset, hiprngStateXORWOW_t* state ){
  return curand_init(seed, subsequence, offset, state);
}

QUALIFIERS float hiprng_normal ( hiprngStateMRG32k3a_t* state ){
  return curand_normal(state);
}

QUALIFIERS float hiprng_normal ( hiprngStatePhilox4_32_10_t* state ){
  return curand_normal(state);
}

QUALIFIERS float hiprng_normal ( hiprngStateXORWOW_t* state ){
  return curand_normal(state);
}

QUALIFIERS float hiprng_uniform ( hiprngStateMRG32k3a_t* state ){
  return curand_uniform(state);
}

QUALIFIERS float hiprng_uniform ( hiprngStatePhilox4_32_10_t* state ){
  return curand_uniform(state);
}

QUALIFIERS float hiprng_uniform ( hiprngStateXORWOW_t* state ){
  return curand_uniform(state);
}
*/

/*
#ifdef __cplusplus
}
#endif*/

