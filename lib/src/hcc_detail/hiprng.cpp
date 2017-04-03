#include "hiprng.h"

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
  return hipHCRNGStatusToHIPStatus(HCRNG_FUNCTION_NOT_IMPLEMENTED);
}
hiprngStatus_t hiprngSetGeneratorOffset(hiprngGenerator_t generator, unsigned long long offset){
  return hipHCRNGStatusToHIPStatus(HCRNG_FUNCTION_NOT_IMPLEMENTED);
}  

hiprngStatus_t hiprngCreateGenerator(hiprngGenerator_t* generator,
                                                   hiprngRngType_t rng_type) {
  if(rng_type == 0) {
   *generator = (hcrngMrg31k3pStreamCreator*)malloc(sizeof(hcrngMrg31k3pStreamCreator));
   *(hcrngMrg31k3pStreamCreator*)*generator = defaultStreamCreator_Mrg31k3p;
   rngtyp = 0;
  }
  else if(rng_type == 1) {
   *generator = (hcrngMrg32k3aStreamCreator*)malloc(sizeof(hcrngMrg32k3aStreamCreator));
   *(hcrngMrg32k3aStreamCreator*)*generator = defaultStreamCreator_Mrg32k3a;
   rngtyp = 1;
  }
  else if(rng_type == 2) {
   *generator = (hcrngLfsr113StreamCreator*)malloc(sizeof(hcrngLfsr113StreamCreator));
   *(hcrngLfsr113StreamCreator*)*generator = defaultStreamCreator_Lfsr113;
   rngtyp = 2;
  }
 else if(rng_type == 3) {
   *generator = (hcrngPhilox432StreamCreator*)malloc(sizeof(hcrngPhilox432StreamCreator));
   *(hcrngPhilox432StreamCreator*)*generator = defaultStreamCreator_Philox432;
   rngtyp = 3;
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
    if(tempPhilox432 != 0) {\
      hcrngPhilox432StreamState baseState;\
      baseState.ctr.H.msb = tempPhilox432;\
      baseState.ctr.H.lsb = tempPhilox432;\
      baseState.ctr.L.msb = tempPhilox432;\
      baseState.ctr.L.lsb = tempPhilox432;\
      baseState.deckIndex = 0;\
    return hipHCRNGStatusToHIPStatus(\
       hcrngPhilox432SetBaseCreatorState((hcrngPhilox432StreamCreator*)generator, &baseState));\
    }\
   else\
      return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);

hiprngStatus_t hiprngSetPseudoRandomGeneratorSeed(
    hiprngGenerator_t generator, unsigned long long seed) {
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
     SetSeedLfsr113()
  }
  else if(rngtyp == 3){
     unsigned int tempPhilox432 = seed;
     SetSeedPhilox432()
  }
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}

__device__ void hiprngMrg32k3aInitGenerator(hiprngMrg32k3aGenerator_t* generator, unsigned long long seed) {
  *generator = (hcrngMrg32k3aStreamCreator*)malloc(sizeof(hcrngMrg32k3aStreamCreator));
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

__device__ void hiprngPhilox432InitGenerator(hiprngPhilox432Generator_t* generator, unsigned long long seed) {
  *generator = (hcrngPhilox432StreamCreator*)malloc(sizeof(hcrngPhilox432StreamCreator));
  *(hcrngPhilox432StreamCreator*)*generator = defaultStreamCreator_Philox432;
  unsigned int tempPhilox432 = seed;
  if(tempPhilox432 != 0) {
      hcrngPhilox432StreamState baseState;
      baseState.ctr.H.msb = tempPhilox432;
      baseState.ctr.H.lsb = tempPhilox432;
      baseState.ctr.L.msb = tempPhilox432;
      baseState.ctr.L.lsb = tempPhilox432;
      baseState.deckIndex = 0;
      hcrngPhilox432SetBaseCreatorState((hcrngPhilox432StreamCreator*)generator, &baseState);
  }
}

#undef SetSeed
#undef SetSeedLfsr113
#undef SetSeedPhilox432


  #define Generate(gt)\
  hcrng##gt##Stream *streams##gt = hcrng##gt##CreateStreams((hcrng##gt##StreamCreator*)generator, num, NULL, NULL); \
  unsigned int *outHost##gt = (unsigned int*)malloc(num * sizeof(unsigned int));\
  hcrngStatus hcStatus##gt = hcrng##gt##RandomUnsignedIntegerArray(streams##gt, 1, 4294967294, num, outHost##gt);\
  hipMemcpy(outputPtr, outHost##gt, num * sizeof(unsigned int), hipMemcpyHostToDevice);\
  free(streams##gt);\
  free(outHost##gt);\
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
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS);
}
#undef Generate

  #define GenerateUniform(gt)\
  hcrng##gt##Stream *streams##gt = hcrng##gt##CreateStreams((hcrng##gt##StreamCreator*)generator, num, NULL, NULL); \
  hcrng##gt##Stream *streams_buffer##gt;\
  hipMalloc((void **)&streams_buffer##gt, num * sizeof(hcrng##gt##Stream));\
  hipMemcpy(streams_buffer##gt, streams##gt, num * sizeof(hcrng##gt##Stream), hipMemcpyHostToDevice);\
  free(streams##gt);\
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomU01Array_single(\
       num, streams_buffer##gt, num, outputPtr);\
  hipFree(streams_buffer##gt);\
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
  return hipHCRNGStatusToHIPStatus(HCRNG_SUCCESS); 
}

__device__ void hiprngMrg32k3aGenerateUniform(hiprngMrg32k3aGenerator_t generator, float* outputPtr, size_t num)
{
  hcrngMrg32k3aStream *streamsMrg32k3agt = hcrngMrg32k3aCreateStreams((hcrngMrg32k3aStreamCreator*)generator, num, NULL, NULL); 
  hcrngMrg32k3aStream *streams_bufferMrg32k3a;
  hipMalloc((void **)&streams_bufferMrg32k3a, num * sizeof(hcrngMrg32k3aStream));
  hipMemcpy(streams_bufferMrg32k3a, streamsMrg32k3a, num * sizeof(hcrngMrg32k3aStream), hipMemcpyHostToDevice);
  free(streamsMrg32k3a);
  hcrngStatus hcStatusMrg32k3a = hcrngMrg32k3aDeviceRandomU01Array_single(
       num, streams_bufferMrg32k3a, num, outputPtr);
  hipFree(streams_bufferMrg32k3a);
}

__device__ void hiprngPhilox432GenerateUniform(hiprngPhilox432Generator_t generator, float* outputPtr, size_t num)
{
  hcrngPhilox432Stream *streamsPhilox432 = hcrngPhilox432CreateStreams((hcrngPhilox432StreamCreator*)generator, num, NULL, NULL); 
  hcrngPhilox432Stream *streams_bufferPhilox432;
  hipMalloc((void **)&streams_bufferPhilox432, num * sizeof(hcrngPhilox432Stream));
  hipMemcpy(streams_bufferPhilox432, streamsPhilox432, num * sizeof(hcrngPhilox432Stream), hipMemcpyHostToDevice);
  free(streamsPhilox432);
  hcrngStatus hcStatusPhilox432 = hcrng##gt##DeviceRandomU01Array_single(
       num, streams_bufferPhilox432, num, outputPtr);
  hipFree(streams_bufferPhilox432);
}  


#undef GenerateUniform

  #define GenerateUniformDouble(gt)\
  hcrng##gt##Stream *streams##gt = hcrng##gt##CreateStreams((hcrng##gt##StreamCreator*)generator, num, NULL, NULL); \
  hcrng##gt##Stream *streams_buffer##gt;\
  hipMalloc((void **)&streams_buffer##gt, num * sizeof(hcrng##gt##Stream));\
  hipMemcpy(streams_buffer##gt, streams##gt, num * sizeof(hcrng##gt##Stream), hipMemcpyHostToDevice);\
  free(streams##gt);\
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomU01Array_double(\
       num, streams_buffer##gt, num, outputPtr);\
  hipFree(streams_buffer##gt);\
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
  hcrng##gt##Stream *streams##gt = hcrng##gt##CreateStreams((hcrng##gt##StreamCreator*)generator, num, NULL, NULL); \
  hcrng##gt##Stream *streams_buffer##gt;\
  hipMalloc((void **)&streams_buffer##gt, num * sizeof(hcrng##gt##Stream));\
  hipMemcpy(streams_buffer##gt, streams##gt, num * sizeof(hcrng##gt##Stream), hipMemcpyHostToDevice);\
  free(streams##gt);\
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomNArray_single(\
       num, streams_buffer##gt, num, mean, stddev, outputPtr);\
  hipFree(streams_buffer##gt);\
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

__device__ void hiprngMrg32k3aGenerateNormal(hiprngMrg32k3aGenerator_t generator, float* outputPtr,
                                                   size_t num, float mean, float stddev){

  hcrngMrg32k3aStream *streamsMrg32k3a = hcrngMrg32k3aCreateStreams((hcrngMrg32k3aStreamCreator*)generator, num, NULL, NULL);
  hcrngMrg32k3aStream *streams_bufferMrg32k3a;
  hipMalloc((void **)&streams_bufferMrg32k3a, num * sizeof(hcrngMrg32k3aStream));
  hipMemcpy(streams_bufferMrg32k3a, streamsMrg32k3a, num * sizeof(hcrngMrg32k3aStream), hipMemcpyHostToDevice);
  free(streamsMrg32k3a);
  hcrngStatus hcStatusMrg32k3a = hcrngMrg32k3aDeviceRandomNArray_single(
       num, streams_bufferMrg32k3a, num, mean, stddev, outputPtr);
  hipFree(streams_bufferMrg32k3a);
}

__device__ void hiprngPhilox432GenerateNormal(hiprngPhilox432Generator_t generator, float* outputPtr,
                                                   size_t num, float mean, float stddev){

  hcrngPhilox432Stream *streamsPhilox432 = hcrng##gt##CreateStreams((hcrngPhilox432StreamCreator*)generator, num, NULL, NULL); 
  hcrngPhilox432Stream *streams_bufferPhilox432;
  hipMalloc((void **)&streams_bufferPhilox432, num * sizeof(hcrngPhilox432Stream));
  hipMemcpy(streams_bufferPhilox432, streamsPhilox432, num * sizeof(hcrngPhilox432Stream), hipMemcpyHostToDevice);
  free(streamsPhilox432);
  hcrngStatus hcStatusPhilox432 = hcrngPhilox432DeviceRandomNArray_single(
       num, streams_bufferPhilox432, num, mean, stddev, outputPtr);
  hipFree(streams_bufferPhilox432);
}

  #undef GenerateNormal


  #define GenerateNormalDouble(gt)\
  hcrng##gt##Stream *streams##gt = hcrng##gt##CreateStreams((hcrng##gt##StreamCreator*)generator, num, NULL, NULL); \
  hcrng##gt##Stream *streams_buffer##gt;\
  hipMalloc((void **)&streams_buffer##gt, num * sizeof(hcrng##gt##Stream));\
  hipMemcpy(streams_buffer##gt, streams##gt, num * sizeof(hcrng##gt##Stream), hipMemcpyHostToDevice);\
  free(streams##gt);\
  hcrngStatus hcStatus##gt = hcrng##gt##DeviceRandomNArray_double(\
       num, streams_buffer##gt, num, mean, stddev, outputPtr);\
  hipFree(streams_buffer##gt);\
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

