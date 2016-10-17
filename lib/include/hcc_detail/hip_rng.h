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
#include <hcRNG/mrg31k3p.h>
#include <hcRNG/mrg32k3a.h>
#include <hcRNG/lfsr113.h>
#include <hcRNG/philox432.h>
#include <hc_am.hpp>
#include <hc.hpp>
#ifdef __cplusplus
extern "C" {
#endif

typedef void *hiprngGenerator_t;
enum hiprngRngType_t {
   HIPRNG_PSEUDO_MRG31K3P, HIPRNG_PSEUDO_MRG32K3A, HIPRNG_PSEUDO_LFSR113, HIPRNG_PSEUDO_PHILOX432 };

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
  if(rng_type == 0) {
  *generator = &defaultStreamCreator_Mrg31k3p;
  #define Mrg31k3p 
  }
  else if(rng_type == 1) {
   *generator = &defaultStreamCreator_Mrg32k3a;
   #define Mrg32k3a 
  }
  else if(rng_type == 2) {
   *generator = &defaultStreamCreator_Lfsr113;
   #define Lfsr113 
  }
 else if(rng_type == 3) {
   *generator = &defaultStreamCreator_Philox432;
   #define Philox432
 }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}
#define SetSeed(gt) \
    if(temp##gt != 0) {\
      hcrng##gt##StreamState baseState;\
    for (size_t i = 0; i < 3; ++i)\
      baseState.g1[i] =  temp##gt;\
    for (size_t i = 0; i < 3; ++i)\
      baseState.g2[i] =  temp##gt;\
    return hipHCRNGStatusToHIPStatus(\
       hcrng##gt##SetBaseCreatorState((hcrng##gt##StreamCreator*)generator, &baseState));\
   }\
   else\
      return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);

#define SetSeedLfsr113() \
    if(tempLfsr113 != 0) {\
      hcrngLfsr113StreamState baseState;\
    for (size_t i = 0; i < 4; ++i)\
      baseState.g[i] =  tempLfsr113;\
    return hipHCRNGStatusToHIPStatus(\
       hcrngLfsr113SetBaseCreatorState((hcrngLfsr113StreamCreator*)generator, &baseState));\
   }\
   else\
      return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);

#define SetSeedPhilox432() \
    hcrngPhilox432StreamState baseState;\
      baseState.ctr.H.msb = tempPhilox432;\
      baseState.ctr.H.lsb = tempPhilox432;\
      baseState.ctr.L.msb = tempPhilox432;\
      baseState.ctr.L.lsb = tempPhilox432;\
    return hipHCRNGStatusToHIPStatus(\
       hcrngPhilox432SetBaseCreatorState((hcrngPhilox432StreamCreator*)generator, &baseState));

inline static hiprngStatus_t hiprngSetPseudoRandomGeneratorSeed(
    hiprngGenerator_t generator, unsigned long long seed) {
  #ifdef Mrg32k3a
    unsigned long tempMrg32k3a = seed;
    SetSeed(Mrg32k3a)
  #endif
  #ifdef Mrg31k3p
     unsigned int tempMrg31k3p = seed;
     SetSeed(Mrg31k3p)
  #endif
  #ifdef Mrg31k3p
     unsigned int tempLfsr113 = seed;
     SetSeedLfsr113()
  #endif
  #ifdef Philox432
     unsigned int tempPhilox432 = seed;
     SetSeedPhilox432()
  #endif

}


  #define GenerateUniform(gt)\
  hcrng##gt##Stream *streams##gt = hcrng##gt##CreateStreams((hcrng##gt##StreamCreator*)generator, num, NULL, NULL); \
  hcrng##gt##Stream *streams_buffer##gt = hc::am_alloc(sizeof(hcrng##gt##Stream) * num, acc[1], 0);\
  hc::am_copy(streams_buffer##gt, streams##gt, num* sizeof(hcrng##gt##Stream));\
  free(streams##gt);\
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomU01Array_single(\
      accl_view, num, streams_buffer##gt, num, outputPtr);\
  hc::am_free(streams_buffer##gt);\
  return hipHCRNGStatusToHIPStatus(hcStatus##gt); 

inline static hiprngStatus_t hiprngGenerateUniform(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num) {
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());
  #ifdef Mrg32k3a
    GenerateUniform(Mrg32k3a)
  #endif
  #ifdef Mrg31k3p
     GenerateUniform(Mrg31k3p)
  #endif
  #ifdef Lfsr113
     GenerateUniform(Lfsr113)
  #endif

  #ifdef Philox432
     GenerateUniform(Philox432)
  #endif
}

  #define GenerateUniformDouble(gt)\
  hcrng##gt##Stream *streams##gt = hcrng##gt##CreateStreams((hcrng##gt##StreamCreator*)generator, num, NULL, NULL); \
  hcrng##gt##Stream *streams_buffer##gt = hc::am_alloc(sizeof(hcrng##gt##Stream) * num, acc[1], 0);\
  hc::am_copy(streams_buffer##gt, streams##gt, num* sizeof(hcrng##gt##Stream));\
  free(streams##gt);\
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomU01Array_double(\
      accl_view, num, streams_buffer##gt, num, outputPtr);\
  hc::am_free(streams_buffer##gt);\
  return hipHCRNGStatusToHIPStatus(hcStatus##gt);

inline static hiprngStatus_t hiprngGenerateUniformDouble(
    hiprngGenerator_t generator, double* outputPtr, size_t num) {
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());
  #ifdef Mrg32k3a
    GenerateUniformDouble(Mrg32k3a)
  #endif
  #ifdef Mrg31k3p
     GenerateUniformDouble(Mrg31k3p)
  #endif
 #ifdef Lfsr113
     GenerateUniformDouble(Lfsr113)
  #endif
 #ifdef Philox432
     GenerateUniformDouble(Philox432)
  #endif
}

  #define GenerateNormal(gt)\
  hcrng##gt##Stream *streams##gt = hcrng##gt##CreateStreams((hcrng##gt##StreamCreator*)generator, num, NULL, NULL); \
  hcrng##gt##Stream *streams_buffer##gt = hc::am_alloc(sizeof(hcrng##gt##Stream) * num, acc[1], 0);\
  hc::am_copy(streams_buffer##gt, streams##gt, num* sizeof(hcrng##gt##Stream));\
  free(streams##gt);\
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomNArray_single(\
      accl_view, num, streams_buffer##gt, num, mean, stddev, outputPtr);\
  hc::am_free(streams_buffer##gt);\
  return hipHCRNGStatusToHIPStatus(hcStatus##gt);


inline static hiprngStatus_t hiprngGenerateNormal(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num, float mean, float stddev) {
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());
  #ifdef Mrg32k3a
    GenerateNormal(Mrg32k3a)
  #endif
  #ifdef Mrg31k3p
     GenerateNormal(Mrg31k3p)
  #endif
  #ifdef Lfsr113
     GenerateNormal(Lfsr113)
  #endif

  #ifdef Philox432
     GenerateNormal(Philox432)
  #endif

}

  #define GenerateNormalDouble(gt)\
  hcrng##gt##Stream *streams##gt = hcrng##gt##CreateStreams((hcrng##gt##StreamCreator*)generator, num, NULL, NULL); \
  hcrng##gt##Stream *streams_buffer##gt = hc::am_alloc(sizeof(hcrng##gt##Stream) * num, acc[1], 0);\
  hc::am_copy(streams_buffer##gt, streams##gt, num* sizeof(hcrng##gt##Stream));\
  free(streams##gt);\
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomNArray_double(\
      accl_view, num, streams_buffer##gt, num, mean, stddev, outputPtr);\
  hc::am_free(streams_buffer##gt);\
  return hipHCRNGStatusToHIPStatus(hcStatus##gt);



inline static hiprngStatus_t hiprngGenerateNormalDouble(
    hiprngGenerator_t generator, double* outputPtr, size_t num, double mean, double stddev) {
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());

  #ifdef Mrg32k3a
    GenerateNormalDouble(Mrg32k3a)
  #endif
  #ifdef Mrg31k3p
     GenerateNormalDouble(Mrg31k3p)
  #endif
  #ifdef Lfsr113
     GenerateNormalDouble(Lfsr113)
  #endif

  #ifdef Philox432
     GenerateNormalDouble(Philox432)
  #endif
}



#ifdef __cplusplus
}
#endif

