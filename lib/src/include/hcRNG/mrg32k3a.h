/*  @file Mrg32k3a.h
*  @brief Specific interface for the Mrg32k3a generator
*  @see hcRNG_template.h
*/

#pragma once
#ifndef MRG32K3A_H
#define MRG32K3A_H
#include "hcRNG.h"
#include <stdio.h>

/*  @brief State type of a Mrg32k3a stream
*  The state is a seed consisting of six unsigned 32-bit integers.
*  @see hcrngStreamState
*/
typedef struct {
	/*! @brief Seed for the first MRG component
	*/
	unsigned long g1[3];
	/*! @brief Seed for the second MRG component
	*/
	unsigned long g2[3];
} hcrngMrg32k3aStreamState;


struct hcrngMrg32k3aStream_ {
	union {
		struct {
			hcrngMrg32k3aStreamState states[3];
		};
		struct {
			hcrngMrg32k3aStreamState current;
			hcrngMrg32k3aStreamState initial;
			hcrngMrg32k3aStreamState substream;
		};
	};
};

/*! @copybrief hcrngStream
*  @see hcrngStream
*/
typedef struct hcrngMrg32k3aStream_ hcrngMrg32k3aStream;

struct hcrngMrg32k3aStreamCreator_;
/*! @copybrief hcrngStreamCreator
*  @see hcrngStreamCreator
*/
typedef struct hcrngMrg32k3aStreamCreator_ hcrngMrg32k3aStreamCreator;

	/*! @copybrief hcrngCopyStreamCreator()
	*  @see hcrngCopyStreamCreator()
	*/
	HCRNGAPI hcrngMrg32k3aStreamCreator* hcrngMrg32k3aCopyStreamCreator(const hcrngMrg32k3aStreamCreator* creator, hcrngStatus* err);

	/*! @copybrief hcrngDestroyStreamCreator()
	*  @see hcrngDestroyStreamCreator()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aDestroyStreamCreator(hcrngMrg32k3aStreamCreator* creator);

	/*! @copybrief hcrngRewindStreamCreator()
	 *  @see hcrngRewindStreamCreator()
	 */
	HCRNGAPI hcrngStatus hcrngMrg32k3aRewindStreamCreator(hcrngMrg32k3aStreamCreator* creator);

	/*! @copybrief hcrngSetBaseCreatorState()
	*  @see hcrngSetBaseCreatorState()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aSetBaseCreatorState(hcrngMrg32k3aStreamCreator* creator, const hcrngMrg32k3aStreamState* baseState);

	/*! @copybrief hcrngChangeStreamsSpacing()
	*  @see hcrngChangeStreamsSpacing()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aChangeStreamsSpacing(hcrngMrg32k3aStreamCreator* creator, int e, int c);

	/*! @copybrief hcrngAllocStreams()
	*  @see hcrngAllocStreams()
	*/
	HCRNGAPI hcrngMrg32k3aStream* hcrngMrg32k3aAllocStreams(size_t count, size_t* bufSize, hcrngStatus* err);

	/*! @copybrief hcrngDestroyStreams()
	*  @see hcrngDestroyStreams()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aDestroyStreams(hcrngMrg32k3aStream* streams);

	/*! @copybrief hcrngCreateOverStreams()
	*  @see hcrngCreateOverStreams()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aCreateOverStreams(hcrngMrg32k3aStreamCreator* creator, size_t count, hcrngMrg32k3aStream* streams);

	/*! @copybrief hcrngCreateStreams()
	*  @see hcrngCreateStreams()
	*/
	HCRNGAPI hcrngMrg32k3aStream* hcrngMrg32k3aCreateStreams(hcrngMrg32k3aStreamCreator* creator, size_t count, size_t* bufSize, hcrngStatus* err);

	/*! @copybrief hcrngCopyOverStreams()
	*  @see hcrngCopyOverStreams()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aCopyOverStreams(size_t count, hcrngMrg32k3aStream* destStreams, const hcrngMrg32k3aStream* srcStreams);

	/*! @copybrief hcrngCopyStreams()
	*  @see hcrngCopyStreams()
	*/
	HCRNGAPI hcrngMrg32k3aStream* hcrngMrg32k3aCopyStreams(size_t count, const hcrngMrg32k3aStream* streams, hcrngStatus* err);

#define hcrngMrg32k3aRandomU01          _HCRNG_TAG_FPTYPE(hcrngMrg32k3aRandomU01)
#define hcrngMrg32k3aRandomInteger      _HCRNG_TAG_FPTYPE(hcrngMrg32k3aRandomInteger)
#define hcrngMrg32k3aRandomU01Array     _HCRNG_TAG_FPTYPE(hcrngMrg32k3aRandomU01Array)
#define hcrngMrg32k3aRandomIntegerArray _HCRNG_TAG_FPTYPE(hcrngMrg32k3aRandomIntegerArray)

	/*! @copybrief hcrngRandomU01()
	*  @see hcrngRandomU01()
	*/
	HCRNGAPI _HCRNG_FPTYPE hcrngMrg32k3aRandomU01(hcrngMrg32k3aStream* stream);
	HCRNGAPI float  hcrngMrg32k3aRandomU01_float (hcrngMrg32k3aStream* stream);
	HCRNGAPI double hcrngMrg32k3aRandomU01_double(hcrngMrg32k3aStream* stream);

	/*! @copybrief hcrngRandomInteger()
	*  @see hcrngRandomInteger()
	*/
	HCRNGAPI int hcrngMrg32k3aRandomInteger(hcrngMrg32k3aStream* stream, int i, int j);
	HCRNGAPI int hcrngMrg32k3aRandomInteger_float (hcrngMrg32k3aStream* stream, int i, int j);
	HCRNGAPI int hcrngMrg32k3aRandomInteger_double(hcrngMrg32k3aStream* stream, int i, int j);

	/*! @copybrief hcrngRandomU01Array()
	*  @see hcrngRandomU01Array()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aRandomU01Array(hcrngMrg32k3aStream* stream, size_t count, _HCRNG_FPTYPE* buffer);
	HCRNGAPI hcrngStatus hcrngMrg32k3aRandomU01Array_float (hcrngMrg32k3aStream* stream, size_t count, float * buffer);
	HCRNGAPI hcrngStatus hcrngMrg32k3aRandomU01Array_double(hcrngMrg32k3aStream* stream, size_t count, double* buffer);

	/*! @copybrief hcrngRandomIntegerArray()
	*  @see hcrngRandomIntegerArray()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aRandomIntegerArray(hcrngMrg32k3aStream* stream, int i, int j, size_t count, int* buffer);
	HCRNGAPI hcrngStatus hcrngMrg32k3aRandomIntegerArray_float (hcrngMrg32k3aStream* stream, int i, int j, size_t count, int* buffer);
	HCRNGAPI hcrngStatus hcrngMrg32k3aRandomIntegerArray_double(hcrngMrg32k3aStream* stream, int i, int j, size_t count, int* buffer);

	/*! @copybrief hcrngRewindStreams()
	*  @see hcrngRewindStreams()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aRewindStreams(size_t count, hcrngMrg32k3aStream* streams);

	/*! @copybrief hcrngRewindSubstreams()
	*  @see hcrngRewindSubstreams()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aRewindSubstreams(size_t count, hcrngMrg32k3aStream* streams);

	/*! @copybrief hcrngForwardToNextSubstreams()
	*  @see hcrngForwardToNextSubstreams()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aForwardToNextSubstreams(size_t count, hcrngMrg32k3aStream* streams);

	/*! @copybrief hcrngMakeSubstreams()
	 *  @see hcrngMakeSubstreams()
	 */
	HCRNGAPI hcrngMrg32k3aStream* hcrngMrg32k3aMakeSubstreams(hcrngMrg32k3aStream* stream, size_t count, size_t* bufSize, hcrngStatus* err);

	/*! @copybrief hcrngMakeOverSubstreams()
	 *  @see hcrngMakeOverSubstreams()
	 */
	HCRNGAPI hcrngStatus hcrngMrg32k3aMakeOverSubstreams(hcrngMrg32k3aStream* stream, size_t count, hcrngMrg32k3aStream* substreams);

        /*! @copybrief hcrngMrg32k3aCopyOverStreamsFromGlobal()
         *  @see hcrngMrg32k3aCopyOverStreamsFromGlobal()
         */
        HCRNGAPI hcrngStatus hcrngMrg32k3aCopyOverStreamsFromGlobal(size_t count, hcrngMrg32k3aStream* destStreams, hcrngMrg32k3aStream* srcStreams);

        /*! @copybrief hcrngMrg32k3aCopyOverStreamsToGlobal()
         *  @see hcrngMrg32k3aCopyOverStreamsToGlobal()
         */
        HCRNGAPI hcrngStatus hcrngMrg32k3aCopyOverStreamsToGlobal(size_t count, hcrngMrg32k3aStream* destStreams, hcrngMrg32k3aStream* srcStreams); 

 
	/*! @copybrief hcrngAdvanceStreams()
	*  @see hcrngAdvanceStreams()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aAdvanceStreams(size_t count, hcrngMrg32k3aStream* streams, int e, int c);

	/*! @copybrief hcrngDeviceRandomU01Array()
	*  @see hcrngDeviceRandomU01Array()
	*/
#ifdef HCRNG_SINGLE_PRECISION
#define hcrngMrg32k3aDeviceRandomU01Array(...) hcrngMrg32k3aDeviceRandomU01Array_(__VA_ARGS__, HC_TRUE)
#else
#define hcrngMrg32k3aDeviceRandomU01Array(...) hcrngMrg32k3aDeviceRandomU01Array_(__VA_ARGS__, HC_FALSE)
#endif

	/** \internal
	 *  @brief Helper function for hcrngMrg32k3aDeviceRandomU01Array()
	 */
	HCRNGAPI hcrngStatus hcrngMrg32k3aDeviceRandomU01Array_single(size_t streamCount,  Concurrency::array_view<hcrngMrg32k3aStream> &streams,
		size_t numberCount, Concurrency::array_view<float> &outBuffer);
        HCRNGAPI hcrngStatus hcrngMrg32k3aDeviceRandomU01Array_double(size_t streamCount,  Concurrency::array_view<hcrngMrg32k3aStream> &streams,
                size_t numberCount, Concurrency::array_view<double> &outBuffer);
/** \endinternal
 */

	/*! @copybrief hcrngWriteStreamInfo()
	*  @see hcrngWriteStreamInfo()
	*/
	HCRNGAPI hcrngStatus hcrngMrg32k3aWriteStreamInfo(const hcrngMrg32k3aStream* stream, FILE *file);

#if 0
	HCRNGAPI hcrngMrg32k3aStream* hcrngMrg32k3aGetStreamByIndex(hcrngMrg32k3aStream* stream, unsigned int index);
#endif


#endif
