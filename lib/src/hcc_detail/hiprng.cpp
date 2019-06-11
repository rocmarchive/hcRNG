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

#include "hiprng.h"
#include "hcRNG/hcRNG.h"
#include "hip/hip_hcc.h"

#define STREAM_COUNT 16384

#ifdef __cplusplus
extern "C" {
#endif

hcrngMrg31k3pStream *streams_bufferMrg31k3p = NULL;
hcrngMrg32k3aStream *streams_bufferMrg32k3a = NULL;
hcrngLfsr113Stream *streams_bufferLfsr113 = NULL;
hcrngPhilox432Stream *streams_bufferPhilox432 = NULL;
hcrngXorwowStream *streams_bufferXorwow = NULL;

hcrngMrg31k3pStream *streamsMrg31k3p = NULL;
hcrngMrg32k3aStream *streamsMrg32k3a = NULL;
hcrngLfsr113Stream *streamsLfsr113 = NULL;
hcrngPhilox432Stream *streamsPhilox432 = NULL;
hcrngXorwowStream *streamsXorwow = NULL;

hiprngStatus_t hipHCRNGStatusToHIPStatus(hcrngStatus hcStatus) {
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

hiprngStatus_t hiprngSetStream(hiprngGenerator_t generator,
                               hipStream_t stream) {
  hc::accelerator_view* pAcclView;
  hipError_t err = hipHccGetAcceleratorView(stream, &pAcclView);
  if (err != hipSuccess) {
    return HIPRNG_INVALID_VALUE;
  }

  if (rngtyp == 0) {
    return hipHCRNGStatusToHIPStatus(
        hcrngMrg31k3pSetAcclView((hcrngMrg31k3pStreamCreator*)generator,
                                 *pAcclView, static_cast<void*>(stream)));
  } else if (rngtyp == 1) {
    return hipHCRNGStatusToHIPStatus(
        hcrngMrg32k3aSetAcclView((hcrngMrg32k3aStreamCreator*)generator,
                                 *pAcclView, static_cast<void*>(stream)));
  } else if (rngtyp == 2) {
    return hipHCRNGStatusToHIPStatus(
        hcrngLfsr113SetAcclView((hcrngLfsr113StreamCreator*)generator,
                                *pAcclView, static_cast<void*>(stream)));
  } else if (rngtyp == 3) {
    return hipHCRNGStatusToHIPStatus(
        hcrngPhilox432SetAcclView((hcrngPhilox432StreamCreator*)generator,
                                  *pAcclView, static_cast<void*>(stream)));
  } else if (rngtyp == 4) {
    return hipHCRNGStatusToHIPStatus(
        hcrngXorwowSetAcclView((hcrngXorwowStreamCreator*)generator,
                               *pAcclView, static_cast<void*>(stream)));
  }

  return hipHCRNGStatusToHIPStatus(HCRNG_INVALID_RNG_TYPE);
}

hiprngStatus_t hiprngSetGeneratorOffset(hiprngGenerator_t generator,
                                        unsigned long long offset) {
  return hipHCRNGStatusToHIPStatus(HCRNG_FUNCTION_NOT_IMPLEMENTED);
}

#define create_generator(gt)                                               \
  hcrng##gt##StreamCreator streamCreator;                                  \
  *generator =                                                             \
      (hcrng##gt##StreamCreator*)malloc(sizeof(hcrng##gt##StreamCreator)); \
  streamCreator = defaultStreamCreator_##gt;                               \
  streamCreator.currentAcclView = accl_view;                               \
  *(hcrng##gt##StreamCreator*)*generator = streamCreator;

#define stream_alloc(gt)                                                   \
  hipHostMalloc((void**)&streams_buffer##gt,                               \
            STREAM_COUNT * sizeof(hcrng##gt##Stream));

hiprngStatus_t hiprngCreateGenerator(hiprngGenerator_t* generator,
                                     hiprngRngType_t rng_type) {
  hipError_t err;
  hc::accelerator_view* accl_view;
  err = hipHccGetAcceleratorView(hipStreamDefault, &accl_view);

  if (rng_type == 0) {
    create_generator(Mrg31k3p);
    rngtyp = 0;
    stream_alloc(Mrg31k3p)
  } else if (rng_type == 1) {
    create_generator(Mrg32k3a);
    rngtyp = 1;
    stream_alloc(Mrg32k3a)
  } else if (rng_type == 2) {
    create_generator(Lfsr113);
    rngtyp = 2;
    stream_alloc(Lfsr113)
  } else if (rng_type == 3) {
    create_generator(Philox432);
    rngtyp = 3;
    stream_alloc(Philox432)
  } else if (rng_type == 4) {
    create_generator(Xorwow);
    rngtyp = 4;
    stream_alloc(Xorwow)
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}

#define SetSeed(gt)                                                            \
  hcrng##gt##StreamCreator* gen = (hcrng##gt##StreamCreator*)generator;        \
  bool reuse = 1;                                                              \
  for (size_t i = 0; i < 3; i++)                                               \
  if (temp##gt == 0 || (temp##gt != gen->initialState.g1[i] ||                 \
      temp##gt != gen->initialState.g2[i])) {                                  \
    reuse = 0;                                                                 \
    break;                                                                     \
  }                                                                            \
  if (!reuse) {                                                                \
  if (temp##gt != 0) {                                                         \
    hcrng##gt##StreamState baseState;                                          \
    for (size_t i = 0; i < 3; ++i) baseState.g1[i] = temp##gt;                 \
    for (size_t i = 0; i < 3; ++i) baseState.g2[i] = temp##gt;                 \
    err = hcrng##gt##SetBaseCreatorState((hcrng##gt##StreamCreator*)generator, \
                                         &baseState);                          \
  }                                                                            \
  hcrng##gt##CreateOverStreams((hcrng##gt##StreamCreator*)generator,           \
                               STREAM_COUNT, streams_buffer##gt);              \
  }

#define SetSeedLfsr113(gt)                                                     \
  hcrng##gt##StreamCreator* gen = (hcrng##gt##StreamCreator*)generator;        \
  bool reuse = 1;                                                              \
  for (size_t i = 0; i < 4; i++)                                               \
  if (temp##gt == 0 || temp##gt != gen->initialState.g[i]) {                   \
    reuse = 0;                                                                 \
    break;                                                                     \
  }                                                                            \
  if (!reuse) {                                                                \
  if (temp##gt != 0) {                                                         \
    hcrng##gt##StreamState baseState;                                          \
    for (size_t i = 0; i < 4; ++i) baseState.g[i] = temp##gt;                  \
    err = hcrng##gt##SetBaseCreatorState(                                      \
        (hcrng##gt##StreamCreator*)generator, &baseState);                     \
  }                                                                            \
  hcrng##gt##CreateOverStreams((hcrng##gt##StreamCreator*)generator,           \
                               STREAM_COUNT, streams_buffer##gt);              \
  }

#define SetSeedPhilox432(gt)                                                   \
  hcrng##gt##StreamCreator* gen = (hcrng##gt##StreamCreator*)generator;        \
  bool reuse =1;                                                               \
  if (temp##gt == 0 ||                                                         \
      (temp##gt != gen->initialState.ctr.H.msb ||                              \
      temp##gt != gen->initialState.ctr.H.lsb ||                               \
      temp##gt != gen->initialState.ctr.L.msb ||                               \
      temp##gt != gen->initialState.ctr.L.lsb))                                \
    reuse = 0;                                                                 \
  if (!reuse) {                                                                \
  if (temp##gt != 0) {                                                         \
    hcrng##gt##StreamState baseState;                                          \
    baseState.ctr.H.msb = temp##gt;                                            \
    baseState.ctr.H.lsb = temp##gt;                                            \
    baseState.ctr.L.msb = temp##gt;                                            \
    baseState.ctr.L.lsb = temp##gt;                                            \
    baseState.deckIndex = 0;                                                   \
    err = hcrng##gt##SetBaseCreatorState(                                      \
        (hcrng##gt##StreamCreator*)generator, &baseState);                     \
  }                                                                            \
  hcrng##gt##CreateOverStreams((hcrng##gt##StreamCreator*)generator,           \
                               STREAM_COUNT, streams_buffer##gt);              \
  }


#define SetSeedXorwow(gt) \
  hcrng##gt##StreamCreator* gen = (hcrng##gt##StreamCreator*)generator;        \
  bool reuse =1;                                                               \
  if(temp##gt != 0) {\
    hcrng##gt##StreamState baseState;                                          \
    unsigned int s0 = ((unsigned int)seed) ^ 0xaad26b49UL;\
    unsigned int s1 = (unsigned int)(seed >> 32) ^ 0xf7dcefddUL;\
    unsigned int t0 = 1099087573UL * s0;\
    unsigned int t1 = 2591861531UL * s1;\
    baseState.d = 6615241 + t1 + t0;\
    baseState.v[0] = 123456789UL + t0;\
    baseState.v[1] = 362436069UL ^ t0;\
    baseState.v[2] = 521288629UL + t1;\
    baseState.v[3] = 88675123UL ^ t1;\
    baseState.v[4] = 5783321UL + t0;\
    err =  hcrng##gt##SetBaseCreatorState((hcrng##gt##StreamCreator*)generator, &baseState);\
  }\
  hcrng##gt##CreateOverStreams((hcrng##gt##StreamCreator*)generator,           \
                               STREAM_COUNT, streams_buffer##gt);              \

hiprngStatus_t hiprngSetPseudoRandomGeneratorSeed(hiprngGenerator_t generator,
                                                  unsigned long long seed) {
  hcrngStatus err;
  if (rngtyp == 0) {
    unsigned long tempMrg31k3p = seed;
    SetSeed(Mrg31k3p)
  } else if (rngtyp == 1) {
    unsigned int tempMrg32k3a = seed;
    SetSeed(Mrg32k3a)
  } else if (rngtyp == 2) {
    unsigned int tempLfsr113 = seed;
    SetSeedLfsr113(Lfsr113)
  } else if (rngtyp == 3) {
    unsigned int tempPhilox432 = seed;
    SetSeedPhilox432(Philox432)
  } else if (rngtyp == 4) {
    unsigned int tempXorwow = seed;
    SetSeedXorwow(Xorwow)
  }
  return hipHCRNGStatusToHIPStatus(err);
}
#undef SetSeed
#undef SetSeedLfsr113
#undef SetSeedPhilox432

#define stream_create(gt)                                                               \
  generator = &defaultStreamCreator_##gt;                                               \
  hipHostMalloc((void**)&streams_buffer##gt,                                            \
            STREAM_COUNT * sizeof(hcrng##gt##Stream));                                  \
  hcrng##gt##CreateOverStreams((hcrng##gt##StreamCreator*)generator,                    \
                               STREAM_COUNT, streams_buffer##gt);

#define Generate(gt)                                                                    \
  if (generator == NULL || streams_buffer##gt == NULL) {                                \
    stream_create(gt)                                                                   \
  }                                                                                     \
  hcrngStatus hcStatus##gt =                                                            \
      hcrng##gt##DeviceRandomUnsignedIntegerArray_single(                               \
          *accl_view, STREAM_COUNT, streams_buffer##gt, num, 1, 4294967294, outputPtr); \
  return hipHCRNGStatusToHIPStatus(hcStatus##gt);

#define Generate_xorwow(gt)\
  if (generator == NULL || streams_buffer##gt == NULL) {                                \
    stream_create(gt)                                                                   \
  }                                                                                     \
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomUnsignedIntegerArray(               \
        *(((hcrng##gt##StreamCreator*)generator)->currentAcclView),                     \
        STREAM_COUNT, streams_buffer##gt, num, outputPtr);                              \
return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerate(hiprngGenerator_t generator,
                              unsigned int* outputPtr, size_t num) {
  hcrngStatus err;
  hc::accelerator_view* accl_view;
  hipHccGetAcceleratorView(hipStreamDefault, &accl_view);
  if (rngtyp == 0) {
    Generate(Mrg31k3p)
  } else if (rngtyp == 1) {
    Generate(Mrg32k3a)
  } else if (rngtyp == 2) {
    Generate(Lfsr113)
  } else if (rngtyp == 3) {
    Generate(Philox432)
  } else if (rngtyp == 4) {
    Generate_xorwow(Xorwow)
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}
#undef Generate

#define GenerateUniform(gt)                                                 \
  if (generator == NULL || streams_buffer##gt == NULL) {                    \
    stream_create(gt)                                                       \
  }                                                                         \
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomU01Array_single(        \
      *accl_view, STREAM_COUNT, streams_buffer##gt, num, outputPtr);        \
  return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerateUniform(hiprngGenerator_t generator,
                                     float* outputPtr, size_t num) {
  hcrngStatus err;
  hc::accelerator_view* accl_view;
  hipHccGetAcceleratorView(hipStreamDefault, &accl_view);
  if (rngtyp == 0) {
    GenerateUniform(Mrg31k3p)
  } else if (rngtyp == 1) {
    GenerateUniform(Mrg32k3a)
  } else if (rngtyp == 2) {
    GenerateUniform(Lfsr113)
  } else if (rngtyp == 3) {
    GenerateUniform(Philox432)
  } else if(rngtyp == 4){
    GenerateUniform(Xorwow)
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}
#undef GenerateUniform

#define GenerateUniformDouble(gt)                                           \
  if (generator == NULL || streams_buffer##gt == NULL) {                    \
    stream_create(gt)                                                       \
  }                                                                         \
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomU01Array_double(        \
      *accl_view, STREAM_COUNT, streams_buffer##gt, num, outputPtr);        \
  return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerateUniformDouble(hiprngGenerator_t generator,
                                           double* outputPtr, size_t num) {
  hcrngStatus err;
  hc::accelerator_view* accl_view;
  hipHccGetAcceleratorView(hipStreamDefault, &accl_view);
  if (rngtyp == 0) {
    GenerateUniformDouble(Mrg31k3p)
  } else if (rngtyp == 1) {
    GenerateUniformDouble(Mrg32k3a)
  } else if (rngtyp == 2) {
    GenerateUniformDouble(Lfsr113)
  } else if (rngtyp == 3) {
    GenerateUniformDouble(Philox432)
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}
#undef GenerateUniformDouble

#define GenerateNormal(gt)                                                         \
  if (generator == NULL || streams_buffer##gt == NULL) {                           \
    stream_create(gt)                                                              \
  }                                                                                \
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomNArray_single(                 \
      *accl_view, STREAM_COUNT, streams_buffer##gt, num, mean, stddev, outputPtr); \
  return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerateNormal(hiprngGenerator_t generator,
                                    float* outputPtr, size_t num, float mean,
                                    float stddev) {
  hcrngStatus err;
  hc::accelerator_view* accl_view;
  hipHccGetAcceleratorView(hipStreamDefault, &accl_view);
  if (rngtyp == 0) {
    GenerateNormal(Mrg31k3p)
  } else if (rngtyp == 1) {
    GenerateNormal(Mrg32k3a)
  } else if (rngtyp == 2) {
    GenerateNormal(Lfsr113)
  } else if (rngtyp == 3) {
    GenerateNormal(Philox432)
  } else if (rngtyp == 4){
    GenerateNormal(Xorwow)
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}
#undef GenerateNormal

#define GenerateNormalDouble(gt)                                                   \
  if (generator == NULL || streams_buffer##gt == NULL) {                           \
    stream_create(gt)                                                              \
  }                                                                                \
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomNArray_double(                 \
      *accl_view, STREAM_COUNT, streams_buffer##gt, num, mean, stddev, outputPtr); \
  return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerateNormalDouble(hiprngGenerator_t generator,
                                          double* outputPtr, size_t num,
                                          double mean, double stddev) {
  hcrngStatus err;
  hc::accelerator_view* accl_view;
  hipHccGetAcceleratorView(hipStreamDefault, &accl_view);
  if (rngtyp == 0) {
    GenerateNormalDouble(Mrg31k3p)
  } else if (rngtyp == 1) {
    GenerateNormalDouble(Mrg32k3a)
  } else if (rngtyp == 2) {
    GenerateNormalDouble(Lfsr113)
  } else if (rngtyp == 3) {
    GenerateNormalDouble(Philox432)
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}
#undef GenerateNormalDouble
#undef stream_create

#define Destroy(gt)                 \
  if (streams_buffer##gt != NULL) { \
    hipHostFree(streams_buffer##gt);    \
    streams_buffer##gt = NULL;      \
  }                                 \
  return hipHCRNGStatusToHIPStatus( \
      hcrng##gt##DestroyStreamCreator((hcrng##gt##StreamCreator*)generator));

hiprngStatus_t hiprngDestroyGenerator(hiprngGenerator_t generator) {
  if (rngtyp == 0) {
    Destroy(Mrg31k3p)
  } else if (rngtyp == 1) {
    Destroy(Mrg32k3a)
  } else if (rngtyp == 2) {
    Destroy(Lfsr113)
  } else if (rngtyp == 3) {
    Destroy(Philox432)
  }
  else if(rngtyp == 4){
    Destroy(Xorwow)
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}

#undef Destroy

#ifdef __cplusplus
}
#endif




