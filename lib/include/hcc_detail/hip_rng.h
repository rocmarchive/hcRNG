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
#include <hip/hip_runtime.h>
#include <hcRNG/hcRNG.h>
#include <hcRNG/mrg31k3p.h>
#include <hcRNG/mrg32k3a.h>
#include <hcRNG/lfsr113.h>
#include <hcRNG/philox432.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *hiprngGenerator_t;
typedef hcrngMrg32k3aStreamCreator *hiprngMrg32k3aGenerator_t;
//typedef hcrngPhilox432StreamCreator *hiprngPhilox432Generator_t;

hiprngStatus_t hipHCRNGStatusToHIPStatus(hcrngStatus hcStatus);

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

__device__ inline void hiprngMrg32k3aInitGenerator(hiprngMrg32k3aGenerator_t* generator, unsigned long long seed) {
  *generator =(hcrngMrg32k3aStreamCreator*) __hip_hc_malloc(sizeof(hcrngMrg32k3aStreamCreator));
  *(hcrngMrg32k3aStreamCreator*)*generator = defaultStreamCreator_Mrg32k3a;
  unsigned int tempMrg32k3a = seed;
  if(tempMrg32k3a != 0) {
      hcrngMrg32k3aStreamState baseState;
    for (size_t i = 0; i < 3; ++i)
      baseState.g1[i] =  tempMrg32k3a;
    for (size_t i = 0; i < 3; ++i)
      baseState.g2[i] =  tempMrg32k3a;
   hcrngMrg32k3aSetBaseCreatorState((hcrngMrg32k3aStreamCreator*)generator, &baseState);
  }
}


//__device__ void hiprngPhilox432InitGenerator(hiprngPhilox432Generator_t* generator, unsigned long long seed);

__device__ inline void hiprngMrg32k3aGenerateUniform(hiprngMrg32k3aGenerator_t generator, float* outputPtr, size_t num)
{
  hcrngMrg32k3aStream *streamsMrg32k3a = hcrngMrg32k3aCreateStreams((hcrngMrg32k3aStreamCreator*)generator, num, NULL, NULL);
  hcrngMrg32k3aStream *streams_bufferMrg32k3a;
  hipMalloc((void **)&streams_bufferMrg32k3a, num * sizeof(hcrngMrg32k3aStream));
  hipMemcpy(streams_bufferMrg32k3a, streamsMrg32k3a, num * sizeof(hcrngMrg32k3aStream), hipMemcpyHostToDevice);
  free(streamsMrg32k3a);
  hcrngStatus hcStatusMrg32k3a = hcrngMrg32k3aDeviceRandomU01Array_single(
       num, streams_bufferMrg32k3a, num, outputPtr);
  hipFree(streams_bufferMrg32k3a);
}

//__device__ void hiprngPhilox432GenerateUniform(hiprngPhilox432Generator_t generator, float* outputPtr, size_t num);

//__device__ void hiprngMrg32k3aGenerateNormal(hiprngMrg32k3aGenerator_t generator, float* outputPtr,
//                                                   size_t num, float mean, float stddev);

//__device__ void hiprngPhilox432GenerateNormal(hiprngPhilox432Generator_t generator, float* outputPtr,
//                                                   size_t num, float mean, float stddev);

//__device__ void dummy();//{
//   int a=10;
//}

#ifdef __cplusplus
}
#endif

