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
#include <iostream>
#include <hip/hip_runtime_api.h>
#include <hcRNG/hcRNG.h>
#include <hcRNG/mrg31k3p.h>
#include <hcRNG/mrg32k3a.h>
#include <hcRNG/lfsr113.h>
#include <hcRNG/philox432.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef void *hiprngGenerator_t;

inline static hiprngStatus_t hipHCRNGStatusToHIPStatus(hcrngStatus hcStatus) {
  switch (hcStatus) {
    case HCRNG_SUCCESS:
      return HIPRNG_STATUS_SUCCESS;
    case HCRNG_OUT_OF_RESOURCES:
      return HIPRNG_STATUS_ALLOCATION_FAILED;
    case HCRNG_INVALID_VALUE:
      return HIPRNG_INVALID_VALUE;
    case HCRNG_INVALID_RNG_TYPE:
      return HIPRNG_STATUS_TYPE_ERROR;
    case HCRNG_INVALID_STREAM_CREATOR:
      return HIPRNG_INVALID_STREAM_CREATOR;
    case HCRNG_INVALID_SEED:
      return HIPRNG_INVALID_SEED;
    case HCRNG_FUNCTION_NOT_IMPLEMENTED:
      return HIPRNG_FUNCTION_NOT_IMPLEMENTED;
    default:
      throw "Unimplemented status";
  }
}


const char* hiprngGetErrorString();
const char* hiprngGetLibraryRoot();

hiprngStatus_t hiprngSetErrorString(int err, const char* msg,
                                                  ...);
static int rngtyp; 

hiprngStatus_t hiprngSetStream(hiprngGenerator_t generator, hipStream_t stream);
      
hiprngStatus_t hiprngSetGeneratorOffset(hiprngGenerator_t generator, unsigned long long offset);

hiprngStatus_t hiprngCreateGenerator(hiprngGenerator_t* generator,
                                                   hiprngRngType_t rng_type);

hiprngStatus_t hiprngSetPseudoRandomGeneratorSeed(
    hiprngGenerator_t generator, unsigned long long seed);

hiprngStatus_t hiprngGenerate(hiprngGenerator_t generator,
                                              unsigned int* outputPtr,
                                                  size_t num);
hiprngStatus_t hiprngGenerateUniform(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num);

hiprngStatus_t hiprngGenerateUniformDouble(
    hiprngGenerator_t generator, double* outputPtr, size_t num);

hiprngStatus_t hiprngGenerateNormal(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num, float mean, float stddev) ;
hiprngStatus_t hiprngGenerateNormalDouble(
    hiprngGenerator_t generator, double* outputPtr, size_t num, double mean, double stddev);
hiprngStatus_t hiprngDestroyGenerator(hiprngGenerator_t generator);
#ifdef __cplusplus
}
#endif

