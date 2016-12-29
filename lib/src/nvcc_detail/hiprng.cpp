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

#include <hiprng.h>

#ifdef __cplusplus
extern "C" {
#endif

 hiprngStatus_t hiprngCreateGenerator(hiprngGenerator_t* generator,
                                                   hiprngRngType_t rng_type) {
  return hipCURANDStatusToHIPStatus(curandCreateGenerator(generator, hipHIPRngTypeToCuRngType(rng_type)));
}

 hiprngStatus_t hiprngSetPseudoRandomGeneratorSeed(
    hiprngGenerator_t generator, unsigned long long seed) {
  return hipCURANDStatusToHIPStatus(
      curandSetPseudoRandomGeneratorSeed(generator, seed));
}
 hiprngStatus_t hiprngSetStream(hiprngGenerator_t generator, hipStream_t stream){
  return hipCURANDStatusToHIPStatus(
      curandSetStream(generator, stream));
}
 hiprngStatus_t hiprngSetGeneratorOffset(hiprngGenerator_t generator, unsigned long long offset){
 return hipCURANDStatusToHIPStatus(
      curandSetGeneratorOffset(generator, offset));
}
 hiprngStatus_t hiprngGenerate(hiprngGenerator_t generator,
                                                   unsigned int* outputPtr,
                                                   size_t num) {
  return hipCURANDStatusToHIPStatus(
      curandGenerate(generator, outputPtr, num));
}
 hiprngStatus_t hiprngGenerateUniform(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num) {
  return hipCURANDStatusToHIPStatus(
      curandGenerateUniform(generator, outputPtr, num));
}
 hiprngStatus_t hiprngGenerateUniformDouble(hiprngGenerator_t generator,
                                                   double* outputPtr,
                                                   size_t num) {
  return hipCURANDStatusToHIPStatus(
      curandGenerateUniformDouble(generator, outputPtr, num));
}
 hiprngStatus_t hiprngGenerateNormal(hiprngGenerator_t generator,
                                                   float* outputPtr,
                                                   size_t num, float mean, float stddev) {
  return hipCURANDStatusToHIPStatus(
      curandGenerateNormal(generator, outputPtr, num, mean, stddev));
}
 hiprngStatus_t hiprngGenerateNormalDouble(hiprngGenerator_t generator,
                                                   double* outputPtr,
                                                   size_t num, double mean, double stddev) {
  return hipCURANDStatusToHIPStatus(
      curandGenerateNormalDouble(generator, outputPtr, num, mean, stddev));
}
 hiprngStatus_t hiprngDestroyGenerator(hiprngGenerator_t generator){ 
  return hipCURANDStatusToHIPStatus(
      curandDestroyGenerator(generator));
}
#ifdef __cplusplus
}
#endif
