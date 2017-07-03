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

  return hipHCRNGStatusToHIPStatus(HCRNG_INVALID_RNG_TYPE);
}

hiprngStatus_t hiprngSetGeneratorOffset(hiprngGenerator_t generator, unsigned long long offset){
  return hipHCRNGStatusToHIPStatus(HCRNG_FUNCTION_NOT_IMPLEMENTED);
}  

#define stream(gt) \
  streams##gt = (hcrng##gt##Stream*)malloc(STREAM_COUNT * sizeof(hcrng##gt##Stream)); \
  hipMalloc((void **)&streams_buffer##gt, STREAM_COUNT * sizeof(hcrng##gt##Stream));\

hiprngStatus_t hiprngCreateGenerator(hiprngGenerator_t* generator,
                                                   hiprngRngType_t rng_type) {
  if(rng_type == 0) {
   *generator = (hcrngMrg31k3pStreamCreator*)malloc(sizeof(hcrngMrg31k3pStreamCreator));
   *(hcrngMrg31k3pStreamCreator*)*generator = defaultStreamCreator_Mrg31k3p;
   rngtyp = 0;
   stream(Mrg31k3p)
  }
  else if(rng_type == 1) {
   *generator = (hcrngMrg32k3aStreamCreator*)malloc(sizeof(hcrngMrg32k3aStreamCreator));
   *(hcrngMrg32k3aStreamCreator*)*generator = defaultStreamCreator_Mrg32k3a;
   rngtyp = 1;
   stream(Mrg32k3a)
  }
  else if(rng_type == 2) {
   *generator = (hcrngLfsr113StreamCreator*)malloc(sizeof(hcrngLfsr113StreamCreator));
   *(hcrngLfsr113StreamCreator*)*generator = defaultStreamCreator_Lfsr113;
   rngtyp = 2;
   stream(Lfsr113)
  }
 else if(rng_type == 3) {
   *generator = (hcrngPhilox432StreamCreator*)malloc(sizeof(hcrngPhilox432StreamCreator));
   *(hcrngPhilox432StreamCreator*)*generator = defaultStreamCreator_Philox432;
   rngtyp = 3;
   stream(Philox432)
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
  return hipHCRNGStatusToHIPStatus(err);
}
#undef SetSeed
#undef SetSeedLfsr113
#undef SetSeedPhilox432

#define Generate(gt)\
hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomUnsignedIntegerArray_single(\
        *accl_view, STREAM_COUNT, streams_buffer##gt, num, 1, 4294967294, outputPtr);\
return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerate(hiprngGenerator_t generator,
                                              unsigned int* outputPtr,
                                                  size_t num) {
  hipError_t err;
  hc::accelerator_view *accl_view;
  err = hipHccGetAcceleratorView(hipStreamDefault, &accl_view);

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
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}
#undef Generate

#define GenerateUniform(gt)\
hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomU01Array_single(\
       *accl_view, STREAM_COUNT, streams_buffer##gt, num, outputPtr);\
return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerateUniform(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num) {
  hipError_t err;
  hc::accelerator_view *accl_view;
  err = hipHccGetAcceleratorView(hipStreamDefault, &accl_view);

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
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS); 
}
#undef GenerateUniform

#define GenerateUniformDouble(gt)\
hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomU01Array_double(\
       *accl_view, STREAM_COUNT, streams_buffer##gt, num, outputPtr);\
return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerateUniformDouble(
    hiprngGenerator_t generator, double* outputPtr, size_t num) {

  hipError_t err;
  hc::accelerator_view *accl_view;
  err = hipHccGetAcceleratorView(hipStreamDefault, &accl_view);

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
       *accl_view, STREAM_COUNT, streams_buffer##gt, num, mean, stddev, outputPtr);\
return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerateNormal(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num, float mean, float stddev) {

  hipError_t err;
  hc::accelerator_view *accl_view;
  err = hipHccGetAcceleratorView(hipStreamDefault, &accl_view);

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
       *accl_view, STREAM_COUNT, streams_buffer##gt, num, mean, stddev, outputPtr);\
return hipHCRNGStatusToHIPStatus(hcStatus##gt);

hiprngStatus_t hiprngGenerateNormalDouble(
    hiprngGenerator_t generator, double* outputPtr, size_t num, double mean, double stddev) {

  hipError_t err;
  hc::accelerator_view *accl_view;
  err = hipHccGetAcceleratorView(hipStreamDefault, &accl_view);

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
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}

#undef Destroy

#ifdef __cplusplus
}
#endif

