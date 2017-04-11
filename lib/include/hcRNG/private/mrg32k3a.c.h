/*! @file Mrg32k3a.c.h
*  @brief Code for the Mrg32k3a generator common to the host and device
*/
#pragma once
#ifndef PRIVATE_MRG32K3A_CH
#define PRIVATE_MRG32K3A_CH

#define Mrg32k3a_M1 4294967087            
#define Mrg32k3a_M2 4294944443             

#define Mrg32k3a_NORM_double 2.328306549295727688e-10
#define Mrg32k3a_NORM_float  2.3283064e-10

#include <hc.hpp>
#include <hc_am.hpp>
#include <hc_math.hpp>
#include "hc_short_vector.hpp"
#include "hcRNG/hcRNG.h"

#include "hcRNG/mrg32k3a.h"
#include "hcRNG/hcRNG.h"
#include "hcRNG/box_muller_transform.h"
#include <iostream>

//using namespace hc;
using namespace hc;
using namespace hc::short_vector;
using namespace std;

#include <stdlib.h>
#define BLOCK_SIZE 256
#define MODULAR_NUMBER_TYPE unsigned long
#define MODULAR_FIXED_SIZE 3
#include "../include/hcRNG/private/modular.c.h"


// hcrngMrg32k3a_A1p76 and hcrngMrg32k3a_A2p76 jump 2^76 steps forward
static
unsigned long hcrngMrg32k3a_A1p76[3][3] = {
	{ 82758667, 1871391091, 4127413238 },
	{ 3672831523, 69195019, 1871391091 },
	{ 3672091415, 3528743235, 69195019 }
};

static
unsigned long hcrngMrg32k3a_A2p76[3][3] = {
	{ 1511326704, 3759209742, 1610795712 },
	{ 4292754251, 1511326704, 3889917532 },
	{ 3859662829, 4292754251, 3708466080 }
};

hcrngStatus hcrngMrg32k3aCopyOverStreams(size_t count, hcrngMrg32k3aStream* destStreams, const hcrngMrg32k3aStream* srcStreams) __attribute__((hc, cpu)) 
{
        //Check params
        if (!destStreams)
                return HCRNG_INVALID_VALUE;
        if (!srcStreams)
                return HCRNG_INVALID_VALUE;

	for (size_t i = 0; i < count; i++)
		destStreams[i] = srcStreams[i];

	return HCRNG_SUCCESS;
}

/*! @brief Advance the rng one step and returns z such that 1 <= z <= Mrg32k3a_M1
*/
static unsigned long hcrngMrg32k3aNextState(hcrngMrg32k3aStreamState* currentState) __attribute__((hc, cpu))
{

	unsigned long* g1 = currentState->g1;
	unsigned long* g2 = currentState->g2;

	long p0, p1;

	/* component 1 */
	p0 = 1403580 * g1[1] - 810728 * g1[0];
	p0 %= Mrg32k3a_M1;
	if (p0 < 0)
		p0 += Mrg32k3a_M1;
	g1[0] = g1[1];
	g1[1] = g1[2];
	g1[2] = p0;

	/* component 2 */
	p1 = 527612 * g2[2] - 1370589 * g2[0];
	p1 %= Mrg32k3a_M2;
	if (p1 < 0)
		p1 += Mrg32k3a_M2;
	g2[0] = g2[1];
	g2[1] = g2[2];
	g2[2] = p1;

	/* combinations */
	if (p0 > p1)
		return (p0 - p1);
	else return (p0 - p1 + Mrg32k3a_M1);
}



// We use an underscore on the r.h.s. to avoid potential recursion with certain
// preprocessors.
#define IMPLEMENT_GENERATE_FOR_TYPE(fptype) \
	\
	fptype hcrngMrg32k3aRandomU01_##fptype(hcrngMrg32k3aStream* stream) __attribute__((hc, cpu)) { \
	    return hcrngMrg32k3aNextState(&stream->current) * Mrg32k3a_NORM_##fptype; \
	} \
	\
        fptype hcrngMrg32k3aRandomN_##fptype(hcrngMrg32k3aStream* stream1, hcrngMrg32k3aStream* stream2, fptype mu, fptype sigma) __attribute__((hc,cpu)) { \
            static fptype z0, z1, i;\
            i++;\
            const fptype two_pi = 2.0 * 3.14159265358979323846;\
            static bool generate;\
            generate =! generate;\
            if (!generate) return z1 * sigma +mu;\
            fptype u1, u2;\
            u1 = hcrngMrg32k3aRandomU01_##fptype(stream1);\
            u2 = hcrngMrg32k3aRandomU01_##fptype(stream2);\
            z0 = sqrt(-2.0 * log((float)u1)) * cos(two_pi * (float)u2);\
            z1 = sqrt(-2.0 * log((float)u1)) * sin(two_pi * (float)u2);\
	    return z0 * sigma + mu; \
	} \
	\
        int hcrngMrg32k3aRandomInteger_##fptype(hcrngMrg32k3aStream* stream,  int i, int j) __attribute__((hc, cpu)) { \
	    return i + (int)((j - i + 1) * hcrngMrg32k3aRandomU01_##fptype(stream)); \
	} \
	\
        unsigned int hcrngMrg32k3aRandomUnsignedInteger_##fptype(hcrngMrg32k3aStream* stream, unsigned int i, unsigned int j) __attribute__((hc, cpu)) { \
            return i + (unsigned int)((j - i + 1) * hcrngMrg32k3aRandomU01_##fptype(stream)); \
        } \
	hcrngStatus hcrngMrg32k3aRandomU01Array_##fptype(hcrngMrg32k3aStream* stream, size_t count, fptype* buffer) __attribute__((hc, cpu)) { \
		for (size_t i = 0; i < count; i++)  \
			buffer[i] = hcrngMrg32k3aRandomU01_##fptype(stream); \
		return HCRNG_SUCCESS; \
	} \
	\
	hcrngStatus hcrngMrg32k3aRandomIntegerArray_##fptype(hcrngMrg32k3aStream* stream, int i, int j, size_t count, int* buffer) __attribute__((hc, cpu)) { \
		for (size_t k = 0; k < count; k++) \
			buffer[k] = hcrngMrg32k3aRandomInteger_##fptype(stream, i, j); \
		return HCRNG_SUCCESS; \
	}\
       hcrngStatus hcrngMrg32k3aRandomUnsignedIntegerArray_##fptype(hcrngMrg32k3aStream* stream, unsigned int i, unsigned int j, size_t count, unsigned int* buffer) __attribute__((hc, cpu)) { \
                for (size_t k = 0; k < count; k++) \
                  buffer[k] = hcrngMrg32k3aRandomUnsignedInteger_##fptype(stream, i, j); \
                return HCRNG_SUCCESS; \
        }

// On the host, implement everything.
// On the device, implement only what is required to avoid hcuttering memory.
#if defined(HCRNG_SINGLE_PRECISION)  
IMPLEMENT_GENERATE_FOR_TYPE(float)
#endif
#if !defined(HCRNG_SINGLE_PRECISION) 
IMPLEMENT_GENERATE_FOR_TYPE(double)
#endif

// Clean up macros, especially to avoid polluting device code.
#undef IMPLEMENT_GENERATE_FOR_TYPE



hcrngStatus hcrngMrg32k3aRewindStreams(size_t count, hcrngMrg32k3aStream* streams) __attribute__((hc, cpu))
{
        if (!streams)
                return HCRNG_INVALID_VALUE;
	//Reset current state to the stream initial state
	for (size_t j = 0; j < count; j++) {
		streams[j].current = streams[j].substream = streams[j].initial;
	}

	return HCRNG_SUCCESS;
}

hcrngStatus hcrngMrg32k3aRewindSubstreams(size_t count, hcrngMrg32k3aStream* streams) __attribute__((hc, cpu))
{
        if (!streams)
                return HCRNG_INVALID_VALUE;
	//Reset current state to the subStream initial state
	for (size_t j = 0; j < count; j++) {
		streams[j].current = streams[j].substream;
	}

	return HCRNG_SUCCESS;
}

hcrngStatus hcrngMrg32k3aForwardToNextSubstreams(size_t count, hcrngMrg32k3aStream* streams) __attribute__((hc, cpu))
{
        if (!streams)
                return HCRNG_INVALID_VALUE;

	for (size_t k = 0; k < count; k++) {
		modMatVec(hcrngMrg32k3a_A1p76, streams[k].substream.g1, streams[k].substream.g1, Mrg32k3a_M1);
		modMatVec(hcrngMrg32k3a_A2p76, streams[k].substream.g2, streams[k].substream.g2, Mrg32k3a_M2);
		streams[k].current = streams[k].substream;
	}

	return HCRNG_SUCCESS;
}

hcrngStatus hcrngMrg32k3aMakeOverSubstreams(hcrngMrg32k3aStream* stream, size_t count, hcrngMrg32k3aStream* substreams) __attribute__((hc, cpu))
{
	for (size_t i = 0; i < count; i++) {
		hcrngStatus err;
		// snapshot current stream into substreams[i]
		err = hcrngMrg32k3aCopyOverStreams(1, &substreams[i], stream);
		if (err != HCRNG_SUCCESS)
		    return err;
		// advance to next substream
		err = hcrngMrg32k3aForwardToNextSubstreams(1, stream);
		if (err != HCRNG_SUCCESS)
		    return err;
	}
	return HCRNG_SUCCESS;
}

static hcrngStatus validateSeed(const hcrngMrg32k3aStreamState* seed) __attribute__((cpu, hc))
{
        // Check that the seeds have valid values
#if defined(__HCC_ACCELERATOR__) && (__HCC_ACCELERATOR__ != 0)
        for (size_t i = 0; i < 3; ++i)
                if (seed->g1[i] >= Mrg32k3a_M1)
                        return HCRNG_INVALID_SEED;

        for (size_t i = 0; i < 3; ++i)
                if (seed->g2[i] >= Mrg32k3a_M2)
                        return HCRNG_INVALID_SEED;

        if (seed->g1[0] == 0 && seed->g1[1] == 0 && seed->g1[2] == 0)
                return HCRNG_INVALID_SEED;

        if (seed->g2[0] == 0 && seed->g2[1] == 0 && seed->g2[2] == 0)
                return HCRNG_INVALID_SEED;
#else        
        for (size_t i = 0; i < 3; ++i)
                if (seed->g1[i] >= Mrg32k3a_M1)
                        return hcrngSetErrorString(HCRNG_INVALID_SEED, "seed.g1[%u] >= Mrg32k3a_M1", i);

        for (size_t i = 0; i < 3; ++i)
                if (seed->g2[i] >= Mrg32k3a_M2)
                        return hcrngSetErrorString(HCRNG_INVALID_SEED, "seed.g2[%u] >= Mrg32k3a_M2", i);

        if (seed->g1[0] == 0 && seed->g1[1] == 0 && seed->g1[2] == 0)
                return hcrngSetErrorString(HCRNG_INVALID_SEED, "seed.g1 = (0,0,0)");

        if (seed->g2[0] == 0 && seed->g2[1] == 0 && seed->g2[2] == 0)
                return hcrngSetErrorString(HCRNG_INVALID_SEED, "seed.g2 = (0,0,0)");
#endif

        return HCRNG_SUCCESS;
}

hcrngStatus hcrngMrg32k3aSetBaseCreatorState(hcrngMrg32k3aStreamCreator* creator, const hcrngMrg32k3aStreamState* baseState) __attribute__((cpu, hc))
{
#if defined(__HCC_ACCELERATOR__) && (__HCC_ACCELERATOR__ != 0)        
        if (creator == NULL)
                return HCRNG_INVALID_STREAM_CREATOR;
        if (baseState == NULL)
                return HCRNG_INVALID_VALUE;
#else        
        if (creator == NULL)
                return hcrngSetErrorString(HCRNG_INVALID_STREAM_CREATOR, "%s(): modifying the default stream creator is forbidden", __func__);
        if (baseState == NULL)
                return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): baseState cannot be NULL", __func__);
#endif
        hcrngStatus err = validateSeed(baseState);

        if (err == HCRNG_SUCCESS) {
                // initialize new creator
                creator->initialState = creator->nextState = *baseState;
        }

        return err;
}

hcrngMrg32k3aStream* hcrngMrg32k3aAllocStreams(size_t count, size_t* bufSize, hcrngStatus* err) __attribute__((cpu, hc))
{
        hcrngStatus err_ = HCRNG_SUCCESS;
        size_t bufSize_ = count * sizeof(hcrngMrg32k3aStream);

        hcrngMrg32k3aStream* buf = (hcrngMrg32k3aStream*)malloc(bufSize_);
#if defined(__HCC_ACCELERATOR__) && (__HCC_ACCELERATOR__ != 0)
        if (buf == NULL) {
                // allocation failed
                err_ = HCRNG_OUT_OF_RESOURCES;
                bufSize_ = 0;
        }
#else	
//        std::vector<hc::accelerator>acc = hc::accelerator::get_all();
//        hcrngMrg32k3aStream* buf = hc::am_alloc(sizeof(bufSize_), acc[1], 0);
        if (buf == NULL) {
                // allocation failed
                err_ = hcrngSetErrorString(HCRNG_OUT_OF_RESOURCES, "%s(): could not allocate memory for streams", __func__);
                bufSize_ = 0;
        }
#endif

        // set buffer size if needed
        if (bufSize != NULL)
                *bufSize = bufSize_;

        // set error status if needed
        if (err != NULL)
                *err = err_;

        return buf;
}


static hcrngStatus Mrg32k3aCreateStream(hcrngMrg32k3aStreamCreator* creator, hcrngMrg32k3aStream* buffer) __attribute__((cpu, hc))
{
        //Check params
#if defined(__HCC_ACCELERATOR__) && (__HCC_ACCELERATOR__ != 0)        
        if (buffer == NULL)
                return HCRNG_INVALID_VALUE;
#else        
         if (buffer == NULL)
                return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): buffer cannot be NULL", __func__);
#endif
        // use default creator if not given
        if (creator == NULL)
                creator = &defaultStreamCreator_Mrg32k3a;

        // initialize stream
        buffer->current = buffer->initial = buffer->substream = creator->nextState;

        // advance next state in stream creator
        modMatVec(creator->nuA1, creator->nextState.g1, creator->nextState.g1, Mrg32k3a_M1);
        modMatVec(creator->nuA2, creator->nextState.g2, creator->nextState.g2, Mrg32k3a_M2);

        return HCRNG_SUCCESS;
}

hcrngStatus hcrngMrg32k3aCreateOverStreams(hcrngMrg32k3aStreamCreator* creator, size_t count, hcrngMrg32k3aStream* streams) __attribute__((cpu, hc))
{
        // iterate over all individual stream buffers
        for (size_t i = 0; i < count; i++) {

                hcrngStatus err = Mrg32k3aCreateStream(creator, &streams[i]);

                // abort on error
                if (err != HCRNG_SUCCESS)
                        return err;
        }

        return HCRNG_SUCCESS;
}

hcrngMrg32k3aStream* hcrngMrg32k3aCreateStreams(hcrngMrg32k3aStreamCreator* creator, size_t count, size_t* bufSize, hcrngStatus* err) __attribute__((cpu, hc))
{
        hcrngStatus err_;
        size_t bufSize_;
        hcrngMrg32k3aStream* streams = hcrngMrg32k3aAllocStreams(count, &bufSize_, &err_);

        if (err_ == HCRNG_SUCCESS)
                err_ = hcrngMrg32k3aCreateOverStreams(creator, count, streams);

        if (bufSize != NULL)
                *bufSize = bufSize_;

        if (err != NULL)
                *err = err_;

        return streams;
}

inline hcrngStatus hcrngMrg32k3aDeviceRandomU01Array_single(size_t streamCount, hcrngMrg32k3aStream* streams,
        size_t numberCount, float* outBuffer, int streamlength, size_t streams_per_thread)
{
#define HCRNG_SINGLE_PRECISION
        //Check params
#if defined(__HCC_ACCELERATOR__) && (__HCC_ACCELERATOR__ != 0)        
        if (streamCount < 1)
                return HCRNG_INVALID_VALUE;
        if (numberCount < 1)
                return HCRNG_INVALID_VALUE;
        if (numberCount % streamCount != 0)
                return HCRNG_INVALID_VALUE;
#else        
        if (streamCount < 1)
                return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): streamCount cannot be less than 1", __func__);
        if (numberCount < 1)
                return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): numberCount cannot be less than 1", __func__);
        if (numberCount % streamCount != 0)
                return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): numberCount must be a multiple of streamCount", __func__);
#endif
        hcrngStatus status = HCRNG_SUCCESS;
        long size = ((streamCount/streams_per_thread) + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
        hc::extent<1> grdExt(size);
        hc::tiled_extent<1> t_ext(grdExt, BLOCK_SIZE);
#if defined(__HCC_ACCELERATOR__) && (__HCC_ACCELERATOR__ != 0)        
          hc::parallel_for_each(t_ext, [ = ] (hc::tiled_index<1> tidx) __attribute__((hc, cpu)) {
          int gid = tidx.global[0];
          if(gid < (streamCount/streams_per_thread)) {
           for(int i =0; i < numberCount/streamCount; i++) {
              if ((i > 0) && (streamlength > 0) && (i % streamlength == 0)) {
               hcrngMrg32k3aForwardToNextSubstreams(streams_per_thread, &streams[streams_per_thread * gid]);
              }
              if ((i > 0) && (streamlength < 0) && (i % streamlength == 0)) {
               hcrngMrg32k3aRewindSubstreams(streams_per_thread, &streams[streams_per_thread * gid]);
              }
              for (int j = 0; j < streams_per_thread; j++)
               outBuffer[streams_per_thread * (i * (streamCount/streams_per_thread) + gid) + j] = hcrngMrg32k3aRandomU01(&streams[streams_per_thread * gid + j]);
              }
           }
        });
#else
          std::vector<hc::accelerator>acc = hc::accelerator::get_all();
          accelerator_view accl_view = (acc[1].get_default_view());
          hc::parallel_for_each(accl_view, t_ext, [ = ] (hc::tiled_index<1> tidx) __attribute__((hc, cpu)) {
          int gid = tidx.global[0];
          if(gid < (streamCount/streams_per_thread)) {
           for(int i =0; i < numberCount/streamCount; i++) {
              if ((i > 0) && (streamlength > 0) && (i % streamlength == 0)) {
               hcrngMrg32k3aForwardToNextSubstreams(streams_per_thread, &streams[streams_per_thread * gid]);
              }
              if ((i > 0) && (streamlength < 0) && (i % streamlength == 0)) {
               hcrngMrg32k3aRewindSubstreams(streams_per_thread, &streams[streams_per_thread * gid]);
              }
              for (int j = 0; j < streams_per_thread; j++)
               outBuffer[streams_per_thread * (i * (streamCount/streams_per_thread) + gid) + j] = hcrngMrg32k3aRandomU01(&streams[streams_per_thread * gid + j]);
              }
           }
        }).wait();
#endif

#undef HCRNG_SINGLE_PRECISION
        return status;
}

inline hcrngStatus hcrngMrg32k3aDeviceRandomNArray_single(size_t streamCount, hcrngMrg32k3aStream *streams,
        size_t numberCount, float mu, float sigma, float *outBuffer, int streamlength, size_t streams_per_thread)
{
#define HCRNG_SINGLE_PRECISION 
        
#if defined(__HCC_ACCELERATOR__) && (__HCC_ACCELERATOR__ != 0)        
        if (streamCount < 1)
                return HCRNG_INVALID_VALUE;
        if (numberCount < 1)
                return HCRNG_INVALID_VALUE;
        if (numberCount % streamCount != 0)
                return HCRNG_INVALID_VALUE;

	hcrngStatus status = hcrngMrg32k3aDeviceRandomU01Array_single(streamCount, streams,numberCount, outBuffer, streamlength, streams_per_thread);
        if (status == HCRNG_SUCCESS){
                status = box_muller_transform_single(mu, sigma, outBuffer, numberCount);
                return status;
            }
#else
        if (streamCount < 1)
                return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): streamCount cannot be less than 1", __func__);
        if (numberCount < 1)
                return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): numberCount cannot be less than 1", __func__);
        if (numberCount % streamCount != 0)
                return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): numberCount must be a multiple of streamCount", __func__);
	hcrngStatus status = hcrngMrg32k3aDeviceRandomU01Array_single(streamCount, streams,numberCount, outBuffer, streamlength, streams_per_thread);
        if (status == HCRNG_SUCCESS){
                status = box_muller_transform_single(mu, sigma, outBuffer, numberCount);
                return status;
            }
#endif
        
#undef HCRNG_SINGLE_PRECISION
        return status;
}

#endif // PRIVATE_Mrg32k3a_CH
