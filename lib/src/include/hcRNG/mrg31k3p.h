/*  @file mrg31k3p.h
 *  @brief Specific interface for the MRG31k3p generator
 *  @see hcRNG_template.h
 */

#pragma once
#ifndef MRG31K3P_H
#define MRG31K3P_H
#include "hcRNG.h"
#include <stdio.h>

/*  @brief State type of a MRG31k3p stream
 *  The state is a seed consisting of six unsigned 32-bit integers.
 *  @see hcrngStreamState
 */
typedef struct {
    /*! @brief Seed for the first MRG component
     */
    unsigned int g1[3];
    /*! @brief Seed for the second MRG component
     */
    unsigned int g2[3];
} hcrngMrg31k3pStreamState;


struct hcrngMrg31k3pStream_ {
	union {
		struct {
			hcrngMrg31k3pStreamState states[3];
		};
		struct {
			hcrngMrg31k3pStreamState current;
			hcrngMrg31k3pStreamState initial;
			hcrngMrg31k3pStreamState substream;
		};
	};
};

/*! @copybrief hcrngStream
 *  @see hcrngStream
 */
typedef struct hcrngMrg31k3pStream_ hcrngMrg31k3pStream;

struct hcrngMrg31k3pStreamCreator_;
/*! @copybrief hcrngStreamCreator
 *  @see hcrngStreamCreator
 */
typedef struct hcrngMrg31k3pStreamCreator_ hcrngMrg31k3pStreamCreator;

/*! @copybrief hcrngCopyStreamCreator()
 *  @see hcrngCopyStreamCreator()
 */
HCRNGAPI hcrngMrg31k3pStreamCreator* hcrngMrg31k3pCopyStreamCreator(const hcrngMrg31k3pStreamCreator* creator, hcrngStatus* err);

/*! @copybrief hcrngDestroyStreamCreator()
 *  @see hcrngDestroyStreamCreator()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pDestroyStreamCreator(hcrngMrg31k3pStreamCreator* creator);

/*! @copybrief hcrngRewindStreamCreator()
 *  @see hcrngRewindStreamCreator()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pRewindStreamCreator(hcrngMrg31k3pStreamCreator* creator);

/*! @copybrief hcrngSetBaseCreatorState()
 *  @see hcrngSetBaseCreatorState()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pSetBaseCreatorState(hcrngMrg31k3pStreamCreator* creator, const hcrngMrg31k3pStreamState* baseState);

/*! @copybrief hcrngChangeStreamsSpacing()
 *  @see hcrngChangeStreamsSpacing()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pChangeStreamsSpacing(hcrngMrg31k3pStreamCreator* creator, int e, int c);

/*! @copybrief hcrngAllocStreams()
 *  @see hcrngAllocStreams()
 */
HCRNGAPI hcrngMrg31k3pStream* hcrngMrg31k3pAllocStreams(size_t count, size_t* bufSize, hcrngStatus* err);

/*! @copybrief hcrngDestroyStreams()
 *  @see hcrngDestroyStreams()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pDestroyStreams(hcrngMrg31k3pStream* streams);

/*! @copybrief hcrngCreateOverStreams()
 *  @see hcrngCreateOverStreams()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pCreateOverStreams(hcrngMrg31k3pStreamCreator* creator, size_t count, hcrngMrg31k3pStream* streams);

/*! @copybrief hcrngCreateStreams()
 *  @see hcrngCreateStreams()
 */
HCRNGAPI hcrngMrg31k3pStream* hcrngMrg31k3pCreateStreams(hcrngMrg31k3pStreamCreator* creator, size_t count, size_t* bufSize, hcrngStatus* err);

/*! @copybrief hcrngCopyOverStreams()
 *  @see hcrngCopyOverStreams()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pCopyOverStreams(size_t count, hcrngMrg31k3pStream* destStreams, const hcrngMrg31k3pStream* srcStreams);

/*! @copybrief hcrngCopyStreams()
 *  @see hcrngCopyStreams()
 */
HCRNGAPI hcrngMrg31k3pStream* hcrngMrg31k3pCopyStreams(size_t count, const hcrngMrg31k3pStream* streams, hcrngStatus* err);

#define hcrngMrg31k3pRandomU01          _HCRNG_TAG_FPTYPE(hcrngMrg31k3pRandomU01)
#define hcrngMrg31k3pRandomInteger      _HCRNG_TAG_FPTYPE(hcrngMrg31k3pRandomInteger)
#define hcrngMrg31k3pRandomU01Array     _HCRNG_TAG_FPTYPE(hcrngMrg31k3pRandomU01Array)
#define hcrngMrg31k3pRandomIntegerArray _HCRNG_TAG_FPTYPE(hcrngMrg31k3pRandomIntegerArray)

/*! @copybrief hcrngRandomU01()
 *  @see hcrngRandomU01()
 */
HCRNGAPI _HCRNG_FPTYPE hcrngMrg31k3pRandomU01(hcrngMrg31k3pStream* stream);
HCRNGAPI float  hcrngMrg31k3pRandomU01_float (hcrngMrg31k3pStream* stream);
HCRNGAPI double hcrngMrg31k3pRandomU01_double(hcrngMrg31k3pStream* stream);

/*! @copybrief hcrngRandomInteger()
 *  @see hcrngRandomInteger()
 */
HCRNGAPI int hcrngMrg31k3pRandomInteger(hcrngMrg31k3pStream* stream, int i, int j);
HCRNGAPI int hcrngMrg31k3pRandomInteger_float (hcrngMrg31k3pStream* stream, int i, int j);
HCRNGAPI int hcrngMrg31k3pRandomInteger_double(hcrngMrg31k3pStream* stream, int i, int j);

/*! @copybrief hcrngRandomU01Array()
 *  @see hcrngRandomU01Array()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pRandomU01Array(hcrngMrg31k3pStream* stream, size_t count, _HCRNG_FPTYPE* buffer);
HCRNGAPI hcrngStatus hcrngMrg31k3pRandomU01Array_float (hcrngMrg31k3pStream* stream, size_t count, float * buffer);
HCRNGAPI hcrngStatus hcrngMrg31k3pRandomU01Array_double(hcrngMrg31k3pStream* stream, size_t count, double* buffer);

/*! @copybrief hcrngRandomIntegerArray()
 *  @see hcrngRandomIntegerArray()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pRandomIntegerArray(hcrngMrg31k3pStream* stream, int i, int j, size_t count, int* buffer);
HCRNGAPI hcrngStatus hcrngMrg31k3pRandomIntegerArray_float (hcrngMrg31k3pStream* stream, int i, int j, size_t count, int* buffer);
HCRNGAPI hcrngStatus hcrngMrg31k3pRandomIntegerArray_double(hcrngMrg31k3pStream* stream, int i, int j, size_t count, int* buffer);

/*! @copybrief hcrngRewindStreams()
 *  @see hcrngRewindStreams()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pRewindStreams(size_t count, hcrngMrg31k3pStream* streams);

/*! @copybrief hcrngRewindSubstreams()
 *  @see hcrngRewindSubstreams()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pRewindSubstreams(size_t count, hcrngMrg31k3pStream* streams);

/*! @copybrief hcrngForwardToNextSubstreams()
 *  @see hcrngForwardToNextSubstreams()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pForwardToNextSubstreams(size_t count, hcrngMrg31k3pStream* streams);

/*! @copybrief hcrngMakeSubstreams()
 *  @see hcrngMakeSubstreams()
 */
HCRNGAPI hcrngMrg31k3pStream* hcrngMrg31k3pMakeSubstreams(hcrngMrg31k3pStream* stream, size_t count, size_t* bufSize, hcrngStatus* err);

/*! @copybrief hcrngMakeOverSubstreams()
 *  @see hcrngMakeOverSubstreams()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pMakeOverSubstreams(hcrngMrg31k3pStream* stream, size_t count, hcrngMrg31k3pStream* substreams);

/*! @copybrief hcrngMrg31k3pCopyOverStreamsFromGlobal()
 *  @see hcrngMrg31k3pCopyOverStreamsFromGlobal()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pCopyOverStreamsFromGlobal(size_t count, hcrngMrg31k3pStream* destStreams, hcrngMrg31k3pStream* srcStreams);

/*! @copybrief hcrngMrg31k3pCopyOverStreamsToGlobal()
 *  @see hcrngMrg31k3pCopyOverStreamsToGlobal()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pCopyOverStreamsToGlobal(size_t count, hcrngMrg31k3pStream* destStreams, hcrngMrg31k3pStream* srcStreams);

/*! @copybrief hcrngAdvanceStreams()
 *  @see hcrngAdvanceStreams()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pAdvanceStreams(size_t count, hcrngMrg31k3pStream* streams, int e, int c);

/*! @copybrief hcrngDeviceRandomU01Array()
 *  @see hcrngDeviceRandomU01Array()
 */
#ifdef HCRNG_SINGLE_PRECISION
#define hcrngMrg31k3pDeviceRandomU01Array(...) hcrngMrg31k3pDeviceRandomU01Array_(__VA_ARGS__, HC_TRUE)
#else
#define hcrngMrg31k3pDeviceRandomU01Array(...) hcrngMrg31k3pDeviceRandomU01Array_(__VA_ARGS__, HC_FALSE)
#endif

/** \internal
 *  @brief Helper function for hcrngMrg31k3pDeviceRandomU01Array()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pDeviceRandomU01Array_single(size_t streamCount, Concurrency::array_view<hcrngMrg31k3pStream> &streams,
	size_t numberCount, Concurrency::array_view<float> &outBuffer);
HCRNGAPI hcrngStatus hcrngMrg31k3pDeviceRandomU01Array_double(size_t streamCount, Concurrency::array_view<hcrngMrg31k3pStream> &streams,
        size_t numberCount, Concurrency::array_view<double> &outBuffer);
/** \endinternal
 */

/*! @copybrief hcrngWriteStreamInfo()
 *  @see hcrngWriteStreamInfo()
 */
HCRNGAPI hcrngStatus hcrngMrg31k3pWriteStreamInfo(const hcrngMrg31k3pStream* stream, FILE *file);


#if 0
HCRNGAPI hcrngMrg31k3pStream* hcrngMrg31k3pGetStreamByIndex(hcrngMrg31k3pStream* stream, unsigned int index);
#endif

#endif
