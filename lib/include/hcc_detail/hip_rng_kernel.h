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

#include <hiprng.h>

#if !defined(QUALIFIERS)
#define QUALIFIERS static __inline__ __device__
#endif

/*
#ifdef __cplusplus
extern "C" {
#endif*/

typedef hiprngGenerator_t hiprngStateMRG32k3a_t;
typedef hiprngGenerator_t hiprngStatePhilox4_32_10_t; 
typedef void *hiprngStateXORWOW_t;

QUALIFIERS void hiprng_init ( unsigned long long seed, unsigned long long subsequence, unsigned long long offset, hiprngStateMRG32k3a_t* state ){
  hiprngMrg32k3aInitGenerator(state, seed);
}

QUALIFIERS void hiprng_init ( unsigned long long seed, unsigned long long subsequence, unsigned long long offset, hiprngStatePhilox4_32_10_t* state ){
  hiprngPhilox432InitGenerator(state, seed);
}

QUALIFIERS void hiprng_init ( unsigned long long seed, unsigned long long subsequence, unsigned long long offset, hiprngStateXORWOW_t* state ){
  //NOT SUPPORTED
}

QUALIFIERS float hiprng_normal ( hiprngStateMRG32k3a_t* state ){
  float outputPtr;
  hiprngMrg32k3aGenerateNormal(*state, &outputPtr, 1, 0, 1);
  return outputPtr;
}

QUALIFIERS float hiprng_normal ( hiprngStatePhilox4_32_10_t* state ){
  float outputPtr;
  hiprngPhilox432GenerateNormal(*state, &outputPtr, 1, 0, 1);
  return outputPtr;
}

QUALIFIERS float hiprng_normal ( hiprngStateXORWOW_t* state ){
  //NOT SUPPORTED
  float outputPtr;
  return outputPtr;
}

QUALIFIERS float hiprng_uniform ( hiprngStateMRG32k3a_t* state ){
  float outputPtr;
  hiprngMrg32k3aGenerateUniform(*state, &outputPtr, 1);
  return outputPtr;
}

QUALIFIERS float hiprng_uniform ( hiprngStatePhilox4_32_10_t* state ){
  float outputPtr;
  hiprngPhilox432GenerateUniform(*state, &outputPtr, 1);
  return outputPtr;
}

QUALIFIERS float hiprng_uniform ( hiprngStateXORWOW_t* state ){
  //NOT SUPPORTED
  float outputPtr;
  return outputPtr;
}

/*
#ifdef __cplusplus
}
#endif
*/
