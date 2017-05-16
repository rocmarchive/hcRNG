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
//#include <hip/hip_runtime_api.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef curandGenerator_t hiprngGenerator_t;
typedef cudaStream_t hipStream_t;

hiprngStatus_t hiprngCreateGenerator(hiprngGenerator_t* generator,
                                                   hiprngRngType_t rng_type);
hiprngStatus_t hiprngSetPseudoRandomGeneratorSeed(
    hiprngGenerator_t generator, unsigned long long seed);
hiprngStatus_t hiprngSetStream(hiprngGenerator_t generator, hipStream_t stream);
hiprngStatus_t hiprngSetGeneratorOffset(hiprngGenerator_t generator, unsigned long long offset);
hiprngStatus_t hiprngGenerate(hiprngGenerator_t generator,
                                                   unsigned int* outputPtr,
                                                   size_t num);
hiprngStatus_t hiprngGenerateUniform(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num);
hiprngStatus_t hiprngGenerateUniformDouble(hiprngGenerator_t generator,
                                                   double* outputPtr,
                                                   size_t num);
hiprngStatus_t hiprngGenerateNormal(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num, float mean, float stddev) ;
hiprngStatus_t hiprngGenerateNormalDouble(hiprngGenerator_t generator,
                                                   double* outputPtr,
                                                   size_t num, double mean, double stddev);

hiprngStatus_t hiprngDestroyGenerator(hiprngGenerator_t generator); 

#ifdef __cplusplus
}
#endif
