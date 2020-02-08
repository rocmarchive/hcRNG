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

#include <hip/hip_runtime_api.h>
#include <hcRNG/hcRNG.h>
#include <hcRNG/mrg31k3p.h>
#include <hcRNG/mrg32k3a.h>
#include <hcRNG/lfsr113.h>
#include <hcRNG/philox432.h>
#include <hcRNG/mtgp32.h>
#include <hcRNG/xorwow.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * \struct MTGP32_PARAMS_FAST_T
 * MTGP32 parameters.
 * Some element is redundant to keep structure simple.
 *
 * pos is a pick up position which is selected to have good
 * performance on graphic processors. pos < Q, where Q is a maximum
 * number such thatthe size of status array - Q is a power of 2.
 * For example, when  mexp is 44497, size of 32-bit status array
 * is 696, and Q is 184, then pos is between 4 and 183. This means
 * 512 parallel calculations is allowed when mexp is 44497.
 *
 * poly_sha1 is SHA1 digest of the characteristic polynomial of
 * state transition function. SHA1 is calculated based on printing
 * form of the polynomial. This is important when we use parameters
 * generated by the dynamic creator which
 *
 * mask This is a mask to make the dimension of state space have
 * just Mersenne Prime. This is redundant.
 */

typedef hcrngStateMtgp32 hiprngStateMtgp32_t;
typedef hcrngStateMtgp32 hiprngStateMtgp32;

typedef mtgp32_kernel_params mtgp32_kernel_params_t;


typedef void *hiprngGenerator_t;

extern hcrngMrg31k3pStream *streams_bufferMrg31k3p;
extern hcrngMrg32k3aStream *streams_bufferMrg32k3a;
extern hcrngLfsr113Stream *streams_bufferLfsr113;
extern hcrngPhilox432Stream *streams_bufferPhilox432;
extern hcrngXorwowStream *streams_bufferXorwow;

extern hcrngMrg31k3pStream *streamsMrg31k3p;
extern hcrngMrg32k3aStream *streamsMrg32k3a;
extern hcrngLfsr113Stream *streamsLfsr113;
extern hcrngPhilox432Stream *streamsPhilox432;
extern hcrngXorwowStream *streamsXorwow;

hiprngStatus_t hipHCRNGStatusToHIPStatus(hcrngStatus hcStatus);

hiprngStatus_t hiprngSetStream(hiprngGenerator_t generator, hipStream_t stream);

hiprngStatus_t hiprngSetGeneratorOffset(hiprngGenerator_t generator,
                                        unsigned long long offset);

hiprngStatus_t hiprngCreateGenerator(hiprngGenerator_t *generator,
                                     hiprngRngType_t rng_type);

hiprngStatus_t hiprngSetPseudoRandomGeneratorSeed(hiprngGenerator_t generator,
                                                  unsigned long long seed);

hiprngStatus_t hiprngGenerate(hiprngGenerator_t generator,
                              unsigned int *outputPtr, size_t num);

hiprngStatus_t hiprngGenerateUniform(hiprngGenerator_t generator,
                                     float *outputPtr, size_t num);

hiprngStatus_t hiprngGenerateUniformDouble(hiprngGenerator_t generator,
                                           double *outputPtr, size_t num);

hiprngStatus_t hiprngGenerateNormal(hiprngGenerator_t generator,
                                    float *outputPtr, size_t num, float mean,
                                    float stddev);
hiprngStatus_t hiprngGenerateNormalDouble(hiprngGenerator_t generator,
                                          double *outputPtr, size_t num,
                                          double mean, double stddev);

hiprngStatus_t hiprngDestroyGenerator(hiprngGenerator_t generator);




#ifdef __cplusplus
}
#endif

