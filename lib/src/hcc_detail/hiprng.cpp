#include "hiprng.h"
#include <hcRNG/hcRNG.h>

#define STREAM_COUNT 16384

#ifdef __cplusplus
extern "C" {
#endif

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

hiprngStatus_t hiprngSetStream(hiprngGenerator_t generator, hipStream_t stream){
  
  hc::accelerator_view *pAcclView;
  hipError_t err = hipHccGetAcceleratorView(stream, &pAcclView);
  if (err != hipSuccess)
  {
    return HIPRNG_INVALID_VALUE;
  }  
  
  if(rngtyp == 0) {
    return hipHCRNGStatusToHIPStatus(hcrngMrg31k3pSetAcclView((hcrngMrg31k3pStreamCreator*)generator, *pAcclView, static_cast<void*>(stream)));
  }
  else if(rngtyp == 1) {
    return hipHCRNGStatusToHIPStatus(hcrngMrg32k3aSetAcclView((hcrngMrg32k3aStreamCreator*)generator, *pAcclView, static_cast<void*>(stream)));
  }
  else if(rngtyp == 2) {
    return hipHCRNGStatusToHIPStatus(hcrngLfsr113SetAcclView((hcrngLfsr113StreamCreator*)generator, *pAcclView, static_cast<void*>(stream)));
  }
  else if(rngtyp == 3) {
    return hipHCRNGStatusToHIPStatus(hcrngPhilox432SetAcclView((hcrngPhilox432StreamCreator*)generator, *pAcclView, static_cast<void*>(stream)));
  }
  else if(rngtyp == 4) {
    return hipHCRNGStatusToHIPStatus(hcrngXorwowSetAcclView((hcrngXorwowStreamCreator*)generator, *pAcclView, static_cast<void*>(stream)));
  }

  return hipHCRNGStatusToHIPStatus(HCRNG_INVALID_RNG_TYPE);
}

hiprngStatus_t hiprngSetGeneratorOffset(hiprngGenerator_t generator, unsigned long long offset){
  return hipHCRNGStatusToHIPStatus(HCRNG_FUNCTION_NOT_IMPLEMENTED);
}  

#define create_generator(gt) \
   hcrng##gt##StreamCreator streamCreator;\
   *generator = (hcrng##gt##StreamCreator*)malloc(sizeof(hcrng##gt##StreamCreator));\
   streamCreator = defaultStreamCreator_##gt;\
   streamCreator.currentAcclView = accl_view;\
   *(hcrng##gt##StreamCreator*)*generator = streamCreator;

#define stream_alloc(gt) \
  streams##gt = (hcrng##gt##Stream*)malloc(STREAM_COUNT * sizeof(hcrng##gt##Stream)); \
  hipMalloc((void **)&streams_buffer##gt, STREAM_COUNT * sizeof(hcrng##gt##Stream));

hiprngStatus_t hiprngCreateGenerator(hiprngGenerator_t* generator,
                                                   hiprngRngType_t rng_type) {
  hipError_t err;
  hc::accelerator_view *accl_view;
  err = hipHccGetAcceleratorView(hipStreamDefault, &accl_view);

  if(rng_type == 0) {
   create_generator(Mrg31k3p);
   rngtyp = 0;
   stream_alloc(Mrg31k3p)
  }
  else if(rng_type == 1) {
   create_generator(Mrg32k3a);
   rngtyp = 1;
   stream_alloc(Mrg32k3a)
  }
  else if(rng_type == 2) {
   create_generator(Lfsr113);
   rngtyp = 2;
   stream_alloc(Lfsr113)
  }
 else if(rng_type == 3) {
   create_generator(Philox432);
   rngtyp = 3;
   stream_alloc(Philox432)
 }
 else if(rng_type == 4) {
   create_generator(Xorwow);
   rngtyp = 4;
   stream_alloc(Xorwow)
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
  err = hcrng##gt##SetBaseCreatorState((hcrng##gt##StreamCreator*)generator, &baseState);\
  }\
  hcrng##gt##CreateOverStreams((hcrng##gt##StreamCreator*)generator, STREAM_COUNT, streams##gt); \
  hipMemcpy(streams_buffer##gt, streams##gt, STREAM_COUNT * sizeof(hcrng##gt##Stream), hipMemcpyHostToDevice);\

#define SetSeedLfsr113(gt) \
  if(tempLfsr113 != 0) {\
    hcrngLfsr113StreamState baseState;\
  for (size_t i = 0; i < 4; ++i)\
    baseState.g[i] =  tempLfsr113;\
  err = hcrngLfsr113SetBaseCreatorState((hcrngLfsr113StreamCreator*)generator, &baseState);\
  }\
  hcrng##gt##CreateOverStreams((hcrng##gt##StreamCreator*)generator, STREAM_COUNT, streams##gt); \
  hipMemcpy(streams_buffer##gt, streams##gt, STREAM_COUNT * sizeof(hcrng##gt##Stream), hipMemcpyHostToDevice);\

#define SetSeedPhilox432(gt) \
  if(tempPhilox432 != 0) {\
    hcrngPhilox432StreamState baseState;\
    baseState.ctr.H.msb = tempPhilox432;\
    baseState.ctr.H.lsb = tempPhilox432;\
    baseState.ctr.L.msb = tempPhilox432;\
    baseState.ctr.L.lsb = tempPhilox432;\
    baseState.deckIndex = 0;\
    err =  hcrngPhilox432SetBaseCreatorState((hcrngPhilox432StreamCreator*)generator, &baseState);\
  }\
  hcrng##gt##CreateOverStreams((hcrng##gt##StreamCreator*)generator, STREAM_COUNT, streams##gt); \
  hipMemcpy(streams_buffer##gt, streams##gt, STREAM_COUNT * sizeof(hcrng##gt##Stream), hipMemcpyHostToDevice);\

#define SetSeedXorwow(gt) \
  if(tempXorwow != 0) {\
    hcrngXorwowStreamState baseState;\
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
    err =  hcrngXorwowSetBaseCreatorState((hcrngXorwowStreamCreator*)generator, &baseState);\
  }\
  hcrng##gt##CreateOverStreams((hcrng##gt##StreamCreator*)generator, STREAM_COUNT, streams##gt); \
  hipMemcpy(streams_buffer##gt, streams##gt, STREAM_COUNT * sizeof(hcrng##gt##Stream), hipMemcpyHostToDevice);\

hiprngStatus_t hiprngSetPseudoRandomGeneratorSeed(
    hiprngGenerator_t generator, unsigned long long seed) {
  hcrngStatus err;
  if(rngtyp == 0){
     unsigned long tempMrg31k3p = seed;
     SetSeed(Mrg31k3p)
  }
  else if(rngtyp == 1){
     unsigned int tempMrg32k3a = seed;
     SetSeed(Mrg32k3a)
  }
  else if(rngtyp == 2){
     unsigned int tempLfsr113 = seed;
     SetSeedLfsr113(Lfsr113)
  }
  else if(rngtyp == 3){
     unsigned int tempPhilox432 = seed;
     SetSeedPhilox432(Philox432)
  }
  else if(rngtyp == 4){
     unsigned int tempXorwow = seed;
     SetSeedXorwow(Xorwow)
  }
  return hipHCRNGStatusToHIPStatus(err);
}
#undef SetSeed
#undef SetSeedLfsr113
#undef SetSeedPhilox432

#define Generate(gt)\
hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomUnsignedIntegerArray_single(\
        *(((hcrng##gt##StreamCreator*)generator)->currentAcclView), STREAM_COUNT, streams_buffer##gt, num, 1, 4294967294, outputPtr);\
return hipHCRNGStatusToHIPStatus(hcStatus##gt);

#define Generate_xorwow(gt)\
hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomUnsignedIntegerArray(\
        *(((hcrng##gt##StreamCreator*)generator)->currentAcclView), STREAM_COUNT, streams_buffer##gt, num, outputPtr);\
return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerate(hiprngGenerator_t generator,
                                              unsigned int* outputPtr,
                                                  size_t num) {
  if(rngtyp == 0){
    Generate(Mrg31k3p)
  }
  else if(rngtyp == 1){
    Generate(Mrg32k3a)
  }
  else if(rngtyp == 2){
    Generate(Lfsr113)
  }
  else if(rngtyp == 3){
    Generate(Philox432)
  }
  else if(rngtyp == 4){
    Generate_xorwow(Xorwow)
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}
#undef Generate

#define GenerateUniform(gt)\
hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomU01Array_single(\
       *(((hcrng##gt##StreamCreator*)generator)->currentAcclView), STREAM_COUNT, streams_buffer##gt, num, outputPtr);\
return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerateUniform(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num) {
  if(rngtyp == 0){
    GenerateUniform(Mrg31k3p)
  }
  else if(rngtyp == 1){
     GenerateUniform(Mrg32k3a)
  }
  else if(rngtyp == 2){
     GenerateUniform(Lfsr113)
  }
  else if(rngtyp == 3){
     GenerateUniform(Philox432)
  }
  else if(rngtyp == 4){
     GenerateUniform(Xorwow)
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS); 
}
#undef GenerateUniform

#define GenerateUniformDouble(gt)\
hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomU01Array_double(\
       *(((hcrng##gt##StreamCreator*)generator)->currentAcclView), STREAM_COUNT, streams_buffer##gt, num, outputPtr);\
return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerateUniformDouble(
    hiprngGenerator_t generator, double* outputPtr, size_t num) {
  if(rngtyp == 0){
    GenerateUniformDouble(Mrg31k3p)
  }
  else if(rngtyp == 1){
     GenerateUniformDouble(Mrg32k3a)
  }
  else if(rngtyp == 2){
     GenerateUniformDouble(Lfsr113)
  }
  else if(rngtyp == 3){
     GenerateUniformDouble(Philox432)
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS); 
}
#undef GenerateUniformDouble

#define GenerateNormal(gt)\
hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomNArray_single(\
       *(((hcrng##gt##StreamCreator*)generator)->currentAcclView), STREAM_COUNT, streams_buffer##gt, num, mean, stddev, outputPtr);\
return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerateNormal(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num, float mean, float stddev) {
  if(rngtyp == 0){
    GenerateNormal(Mrg31k3p)
  }
  else if(rngtyp == 1){
     GenerateNormal(Mrg32k3a)
  }
  else if(rngtyp == 2){
     GenerateNormal(Lfsr113)
  }
  else if(rngtyp == 3){
     GenerateNormal(Philox432)
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS); 
}
#undef GenerateNormal

#define GenerateNormalDouble(gt)\
hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomNArray_double(\
       *(((hcrng##gt##StreamCreator*)generator)->currentAcclView), STREAM_COUNT, streams_buffer##gt, num, mean, stddev, outputPtr);\
return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerateNormalDouble(
    hiprngGenerator_t generator, double* outputPtr, size_t num, double mean, double stddev) {
  if(rngtyp == 0){
    GenerateNormalDouble(Mrg31k3p)
  }
  else if(rngtyp == 1){
     GenerateNormalDouble(Mrg32k3a)
  }
  else if(rngtyp == 2){
     GenerateNormalDouble(Lfsr113)
  }
  else if(rngtyp == 3){
     GenerateNormalDouble(Philox432)
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS); 
}
#undef GenerateNormalDouble

#define Destroy(gt)\
if (streams##gt != NULL){\
  free(streams##gt);\
  streams##gt = NULL;\
}\
if (streams_buffer##gt != NULL){\
  hipFree(streams_buffer##gt);\
  streams_buffer##gt = NULL;\
}\
return hipHCRNGStatusToHIPStatus(hcrng##gt##DestroyStreamCreator((hcrng##gt##StreamCreator*)generator));

hiprngStatus_t hiprngDestroyGenerator(hiprngGenerator_t generator){
  
  if(rngtyp == 0){
    Destroy(Mrg31k3p)
  }
  else if(rngtyp == 1){
    Destroy(Mrg32k3a)
  }
  else if(rngtyp == 2){
    Destroy(Lfsr113)
  }
  else if(rngtyp == 3){
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

