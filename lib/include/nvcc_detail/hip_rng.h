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

#include <cuda_runtime_api.h>
#include <curand.h>
#include <curand_kernel.h>

// HGSOS for Kalmar leave it as C++, only cuRAND needs C linkage.

#ifdef __cplusplus
extern "C" {
#endif
typedef curandGenerator_t hiprngGenerator_t;
typedef curandRngType_t hiprngRngType_t;
 
inline static hiprngStatus_t hipCURANDStatusToHIPStatus(curandStatus_t hcStatus) {
  switch (hcStatus) {
    case CURAND_STATUS_SUCCESS:
      return HIPRNG_SUCCESS;
    case CURAND_STATUS_ALLOCATION_FAILED:
      return HIPRNG_OUT_OF_RESOURCES;
    case CURAND_STATUS_INITIALIZATION_FAILED:
      return HIPRNG_INITIALIZATION_FAILED;
    case CURAND_STATUS_TYPE_ERROR:
      return HIPRNG_INVALID_RNG_TYPE;
    case CURAND_STATUS_VERSION_MISMATCH:
      return HIPRNG_VERSION_MISMATCH;
    case CURAND_STATUS_INTERNAL_ERROR:
      return HIPRNG_FUNCTION_NOT_IMPLEMENTED;
    default:
      throw "Unimplemented status";
  }
}
inline static hiprngStatus_t hiprngCreateGenerator(hiprngGenerator_t* generator,
                                                   hiprngRngType_t rng_type) {
  return hipCURANDStatusToHIPStatus(curandCreateGenerator(generator, rng_type));
}

inline static hiprngStatus_t hiprngSetPseudoRandomGeneratorSeed(
    hiprngGenerator_t generator, unsigned long long seed) {
  return hipCURANDStatusToHIPStatus(
      curandSetPseudoRandomGeneratorSeed(generator, seed));
}
inline static hiprngStatus_t hiprngGenerate(hiprngGenerator_t generator,
                                                   unsigned int* outputPtr,
                                                   size_t num) {
  return hipCURANDStatusToHIPStatus(
      curandGenerate(generator, outputPtr, num));
}
inline static hiprngStatus_t hiprngGenerateUniform(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num) {
  return hipCURANDStatusToHIPStatus(
      curandGenerateUniform(generator, outputPtr, num));
}
inline static hiprngStatus_t hiprngGenerateUniformDouble(hiprngGenerator_t generator,
                                                   double* outputPtr,
                                                   size_t num) {
  return hipCURANDStatusToHIPStatus(
      curandGenerateUniformDouble(generator, outputPtr, num));
}
inline static hiprngStatus_t hiprngGenerateNormal(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num, float mean, float stddev) {
  return hipCURANDStatusToHIPStatus(
      curandGenerateNormal(generator, outputPtr, num, mean, stddev));
}
inline static hiprngStatus_t hiprngGenerateNormalDouble(hiprngGenerator_t generator,
                                                   double* outputPtr,
                                                   size_t num, double mean, double stddev) {
  return hipCURANDStatusToHIPStatus(
      curandGenerateNormalDouble(generator, outputPtr, num, mean, stddev));
}
inline static hiprngStatus_t hiprngDestroyGenerator(hiprngGenerator_t generator){ 
  return hipCURANDStatusToHIPStatus(
      curandDestroyGenerator(generator));
}
#ifdef __cplusplus
}
#endif
