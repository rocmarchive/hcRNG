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
#include <hip_runtime_api.h>
#include <hcRNG/hcRNG.h>
#include <hc_am.hpp>
#include <hc.hpp>
#include "../src/generators/mrg32k3a/mrg32k3a.cpp"
#ifdef __cplusplus
extern "C" {
#endif

typedef hcrngMrg32k3aStreamCreator *hiprngGenerator_t;

enum hiprngRngType_t {
   HCRNG_PSEUDO_MRG32K3A,  HCRNG_PSEUDO_PHILOX432 };


inline static hiprngStatus_t hipHCRNGStatusToHIPStatus(hcrngStatus hcStatus) {
  switch (hcStatus) {
    case HCRNG_SUCCESS:
      return HIPRNG_SUCCESS;
    case HCRNG_OUT_OF_RESOURCES:
      return HIPRNG_OUT_OF_RESOURCES;
    case HCRNG_INVALID_VALUE:
      return HIPRNG_INVALID_VALUE;
    case HCRNG_INVALID_RNG_TYPE:
      return HIPRNG_INVALID_RNG_TYPE;
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

inline static const char* hiprngGetErrorString() {
    return hcrngGetErrorString();
}

inline static const char* hiprngGetLibraryRoot() {
  return hcrngGetLibraryRoot();
}

inline static hiprngStatus_t hiprngSetErrorString(int err, const char* msg,
                                                  ...) {
  return hipHCRNGStatusToHIPStatus(hcrngSetErrorString(err, msg));
}



inline static hiprngStatus_t hiprngCreateGenerator(hiprngGenerator_t* generator,
                                                   hiprngRngType_t rng_type) {

  *generator = &defaultStreamCreator;
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}


inline static hiprngStatus_t hiprngSetPseudoRandomGeneratorSeed(
    hiprngGenerator_t generator, unsigned long long seed) {
    unsigned long temp = seed;
    if(seed != 0) {
      hcrngMrg32k3aStreamState baseState;
    for (size_t i = 0; i < 3; ++i)
      baseState.g1[i] =  temp;
    for (size_t i = 0; i < 3; ++i)
      baseState.g2[i] =  temp;
    return hipHCRNGStatusToHIPStatus(
       hcrngMrg32k3aSetBaseCreatorState(generator, &baseState));
   }
   else
      return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}


inline static hiprngStatus_t hiprngGenerateUniform(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num) {
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());
  hcrngMrg32k3aStream *streams = hcrngMrg32k3aCreateStreams(generator, num, NULL, NULL);
  hcrngMrg32k3aStream *streams_buffer = hc::am_alloc(sizeof(hcrngMrg32k3aStream) * num, acc[1], 0);
  hc::am_copy(streams_buffer, streams, num* sizeof(hcrngMrg32k3aStream));
  free(streams);
  hcrngStatus hcStatus = hcrngMrg32k3aDeviceRandomU01Array_single(
      accl_view, num, streams_buffer, num, outputPtr);
  hc::am_free(streams_buffer);
  return hipHCRNGStatusToHIPStatus(hcStatus);
}

inline static hiprngStatus_t hiprngGenerateUniformDouble(
    hiprngGenerator_t generator, double* outputPtr, size_t num) {
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());
  hcrngMrg32k3aStream *streams = hcrngMrg32k3aCreateStreams(generator, num, NULL, NULL);
  hcrngMrg32k3aStream *streams_buffer = hc::am_alloc(sizeof(hcrngMrg32k3aStream) * num, acc[1], 0);
  hc::am_copy(streams_buffer, streams, num* sizeof(hcrngMrg32k3aStream));
  free(streams);
  hcrngStatus hcStatus = hcrngMrg32k3aDeviceRandomU01Array_double(
      accl_view, num, streams_buffer, num, outputPtr);
  hc::am_free(streams_buffer);
  return hipHCRNGStatusToHIPStatus(hcStatus);
}

inline static hiprngStatus_t hiprngGenerateNormal(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num, float mean, float stddev) {
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());
  hcrngMrg32k3aStream *streams = hcrngMrg32k3aCreateStreams(generator, num, NULL, NULL);
  hcrngMrg32k3aStream *streams_buffer = hc::am_alloc(sizeof(hcrngMrg32k3aStream) * num, acc[1], 0);
  hc::am_copy(streams_buffer, streams, num* sizeof(hcrngMrg32k3aStream));
  free(streams);
  hcrngStatus hcStatus = hcrngMrg32k3aDeviceRandomNArray_single(
      accl_view, num, streams_buffer, num, mean, stddev, outputPtr);
  hc::am_free(streams_buffer);
  return hipHCRNGStatusToHIPStatus(hcStatus);
}

inline static hiprngStatus_t hiprngGenerateNormalDouble(
    hiprngGenerator_t generator, double* outputPtr, size_t num, double mean, double stddev) {
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());
  hcrngMrg32k3aStream *streams = hcrngMrg32k3aCreateStreams(generator, num, NULL, NULL);
  hcrngMrg32k3aStream *streams_buffer = hc::am_alloc(sizeof(hcrngMrg32k3aStream) * num, acc[1], 0);
  hc::am_copy(streams_buffer, streams, num* sizeof(hcrngMrg32k3aStream));
  free(streams);
  hcrngStatus hcStatus = hcrngMrg32k3aDeviceRandomNArray_double(
      accl_view, num, streams_buffer, num, mean, stddev, outputPtr);
  hc::am_free(streams_buffer);
  return hipHCRNGStatusToHIPStatus(hcStatus);
}



#ifdef __cplusplus
}
#endif

