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
#include <hcRNG/mrg32k3a.h>
#include <hcRNG/philox432.h>
// HGSOS for Kalmar leave it as C++, only cuRAND needs C linkage.

#ifdef __cplusplus
extern "C" {
#endif

typedef hcrngMrg32k3aStreamCreator* hiprngGenerator_t;
typedef const char* hiprngRngType_t;
// Number oi streams Size  (use 1 for a single stream object).
size_t streamCount = 1;
#define MRG32K3A
#ifdef MRG32K3A
hcrngMrg32k3aStream *streams = hcrngMrg32k3aCreateStreams(NULL, streamCount, NULL, NULL);
#else
hcrngPhilox432Stream *streams = hcrngPhilox432CreateStreams(NULL, streamCount, NULL, NULL);
#endif
std::string str;
std::vector<hc::accelerator> acc = hc::accelerator::get_all();
accelerator_view accl_view = (acc[1].create_view());

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

inline static hiprngStatus_t hiprngCreateGenerator(hiprngGenerator_t generator,
                                                   hiprngRngType_t rng_type) {
  str = (string)rng_type;
  if (str.substr(18) == "MRG32K3A") {
  return hipHCRNGStatusToHIPStatus(
      hcrngMrg32k3aCreateOverStreams(generator, streamCount, streams));
}
   else {
   return hipHCRNGStatusToHIPStatus(
      hcrngPhilox432CreateOverStreams(generator, streamCount, streams)); 
}
}

inline static hiprngStatus_t hiprngSetPseudoRandomGeneratorSeed(
    hiprngGenerator_t generator, unsigned long long seed) {
  const hcrngMrg32k3aStreamState* baseState =
      (hcrngMrg32k3aStreamState)(&((unsigned long)seed));
  const hcrngPhilox432StreamState* baseState =
      (hcrngPhilox432StreamState)(&((unsigned long)seed));
  if (str.substr(18) == "MRG32K3A") {  
  return hipHCRNGStatusToHIPStatus(
      hcrngMrg32k3aSetBaseCreatorState(generator, baseState));
      }
  else {
   return hipHCRNGStatusToHIPStatus(
      hcrngPhilox432SetBaseCreatorState(generator, baseState)); 
      }
}

inline static hiprngStatus_t hiprngGenerateUniform(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num) {
  if (str.substr(18) == "MRG32K3A") {                                                    
  return hipHCRNGStatusToHIPStatus(hcrngMrg32k3aDeviceRandomU01Array_single(
      accl_view, streamCount, streams, num, outputPtr));
      }
  else {
  return hipHCRNGStatusToHIPStatus(hcrngPhilox432DeviceRandomU01Array_single(
      accl_view, streamCount, streams, num, outputPtr));
      } 
}

inline static hiprngStatus_t hiprngGenerateUniformDouble(
    hiprngGenerator_t generator, double* outputPtr, size_t num) {
  if (str.substr(18) == "MRG32K3A") {    
  return hipHCRNGStatusToHIPStatus(hcrngMrg32k3aDeviceRandomU01Array_double(
      accl_view, streamCount, streams, num, outputPtr));
      }
  else {
    return hipHCRNGStatusToHIPStatus(hcrngPhilox432DeviceRandomU01Array_double(
      accl_view, streamCount, streams, num, outputPtr));
    }
}

#ifdef __cplusplus
}
#endif

