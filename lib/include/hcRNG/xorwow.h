/*  @file Xorwow.h
*  @brief Specific interface for the Xorwow generator
*/

#pragma once
#ifndef XORWOW_H
#define XORWOW_H
#include "hcRNG.h"
#include <stdio.h>

namespace hc {
  class accelerator;
  class accelerator_view;
};

/*  @brief State type of a Xorwow stream
*  The state is a seed consisting of six unsigned 32-bit integers.
*  @see hcrngStreamState
*/
typedef struct {
    unsigned long d;
    unsigned long v[5];
} hcrngXorwowStreamState;


struct hcrngXorwowStream_ {
	union {
		struct {
			hcrngXorwowStreamState states[3];
		};
		struct {
			hcrngXorwowStreamState current;
			hcrngXorwowStreamState initial;
			hcrngXorwowStreamState substream;
		};
	};
};

/*! @copybrief hcrngStream
*  @see hcrngStream
*/
typedef struct hcrngXorwowStream_ hcrngXorwowStream;

//struct hcrngXorwowStreamCreator_;
/*! @copybrief hcrngStreamCreator
*  @see hcrngStreamCreator
*/

struct hcrngXorwowStreamCreator_ {
        hcrngXorwowStreamState initialState;
        hcrngXorwowStreamState nextState;

	bool initialized;
        hc::accelerator *currentAccl;
        hc::accelerator_view *currentAcclView;
        // StreamInfo
        void* currentStream;
};

/*! @brief Default initial seed of the first stream
*/
#define BASE_CREATOR_STATE_XORWOW { 6615241, { 123456789, 362436069, 521288629, 88675123, 5783321 } }

/*! @brief Default stream creator (defaults to \f$2^{134}\f$ steps forward)
*
*  Contains the default seed and the transition matrices to jump \f$\nu\f$ steps forward;
*  adjacent streams are spaced nu steps apart.
*  The default is \f$nu = 2^{134}\f$.
*  The default seed is \f$(12345,12345,12345,12345,12345,12345)\f$.
*/
typedef struct hcrngXorwowStreamCreator_ hcrngXorwowStreamCreator;

static hcrngXorwowStreamCreator defaultStreamCreator_Xorwow = {
        BASE_CREATOR_STATE_XORWOW,
        BASE_CREATOR_STATE_XORWOW
};

	hcrngStatus hcrngXorwowSetAcclView(hcrngXorwowStreamCreator* creator, hc::accelerator_view accl_view, void* stream = nullptr);

	hcrngStatus hcrngXorwowGetAcclView(hcrngXorwowStreamCreator* creator, hc::accelerator_view *&accl_view, void** stream);

	/*! @copybrief hcrngDestroyStreamCreator()
	*  @see hcrngDestroyStreamCreator()
	*/
	HCRNGAPI hcrngStatus hcrngXorwowDestroyStreamCreator(hcrngXorwowStreamCreator* creator);

	/*! @copybrief hcrngSetBaseCreatorState()
	*  @see hcrngSetBaseCreatorState()
	*/
	HCRNGAPI hcrngStatus hcrngXorwowSetBaseCreatorState(hcrngXorwowStreamCreator* creator, const hcrngXorwowStreamState* baseState);

	/*! @copybrief hcrngAllocStreams()
	*  @see hcrngAllocStreams()
	*/
	HCRNGAPI hcrngXorwowStream* hcrngXorwowAllocStreams(size_t count, size_t* bufSize, hcrngStatus* err);

	/*! @copybrief hcrngDestroyStreams()
	*  @see hcrngDestroyStreams()
	*/
	HCRNGAPI hcrngStatus hcrngXorwowDestroyStreams(hcrngXorwowStream* streams);

	/*! @copybrief hcrngCreateOverStreams()
	*  @see hcrngCreateOverStreams()
	*/
	HCRNGAPI hcrngStatus hcrngXorwowCreateOverStreams(hcrngXorwowStreamCreator* creator, size_t count, hcrngXorwowStream* streams);

	/*! @copybrief hcrngCreateStreams()
	*  @see hcrngCreateStreams()
	*/
	HCRNGAPI hcrngXorwowStream* hcrngXorwowCreateStreams(hcrngXorwowStreamCreator* creator, size_t count, size_t* bufSize, hcrngStatus* err);

#define hcrngXorwowRandomU01          _HCRNG_TAG_FPTYPE(hcrngXorwowRandomU01)
#define hcrngXorwowRandomN            _HCRNG_TAG_FPTYPE(hcrngXorwowRandomN)           //Normal distribution 
#define hcrngXorwowRandomInteger      _HCRNG_TAG_FPTYPE(hcrngXorwowRandomInteger)
#define hcrngXorwowRandomUnsignedInteger      _HCRNG_TAG_FPTYPE(hcrngXorwowRandomUnsignedInteger)
#define hcrngXorwowRandomU01Array     _HCRNG_TAG_FPTYPE(hcrngXorwowRandomU01Array)
#define hcrngXorwowRandomIntegerArray _HCRNG_TAG_FPTYPE(hcrngXorwowRandomIntegerArray)
#define hcrngXorwowRandomUnsignedIntegerArray _HCRNG_TAG_FPTYPE(hcrngXorwowRandomUnsignedIntegerArray)

	/*! @copybrief hcrngRandomU01()
	*  @see hcrngRandomU01()
	*/
	HCRNGAPI unsigned int hcrngXorwowRandomU01_float (hcrngXorwowStream* stream);
	HCRNGAPI unsigned int hcrngXorwowRandomU01_double(hcrngXorwowStream* stream);
        
        // Normal distribution
        
	HCRNGAPI _HCRNG_FPTYPE hcrngXorwowRandomN(hcrngXorwowStream* stream1, hcrngXorwowStream* stream2, _HCRNG_FPTYPE mu, _HCRNG_FPTYPE sigma);
	HCRNGAPI float  hcrngXorwowRandomN_float (hcrngXorwowStream* stream, hcrngXorwowStream* stream2, float mu, float sigma);
	HCRNGAPI double hcrngXorwowRandomN_double(hcrngXorwowStream* stream, hcrngXorwowStream* stream2, double mu, double sigma);


	/*! @copybrief hcrngRandomInteger()
	*  @see hcrngRandomInteger()
	*/
	HCRNGAPI  int hcrngXorwowRandomInteger(hcrngXorwowStream* stream,  int i,  int j);
	HCRNGAPI  int hcrngXorwowRandomInteger_float (hcrngXorwowStream* stream, int i, int j);
	HCRNGAPI  int hcrngXorwowRandomInteger_double(hcrngXorwowStream* stream, int i, int j);
        

        HCRNGAPI unsigned int hcrngXorwowRandomUnsignedInteger(hcrngXorwowStream* stream);
        HCRNGAPI unsigned int hcrngXorwowRandomUnsignedInteger_float (hcrngXorwowStream* stream);
        HCRNGAPI unsigned int hcrngXorwowRandomUnsignedInteger_double(hcrngXorwowStream* stream);

#ifdef HCRNG_SINGLE_PRECISION
#define hcrngXorwowDeviceRandomU01Array(...) hcrngXorwowDeviceRandomU01Array_(__VA_ARGS__, HC_TRUE)
#else
#define hcrngXorwowDeviceRandomU01Array(...) hcrngXorwowDeviceRandomU01Array_(__VA_ARGS__, HC_FALSE)
#endif

	/** \internal
	 *  @brief Helper function for hcrngXorwowDeviceRandomU01Array()
	 */
	HCRNGAPI hcrngStatus hcrngXorwowDeviceRandomU01Array_single(hc::accelerator_view accl_view, size_t streamCount,  hcrngXorwowStream* streams,
		size_t numberCount, float* outBuffer, int streamlength = 0, size_t streams_per_thread = 1);
        HCRNGAPI hcrngStatus hcrngXorwowDeviceRandomU01Array_double(hc::accelerator_view accl_view, size_t streamCount,  hcrngXorwowStream* streams,
                size_t numberCount, double* outBuffer, int streamlength = 0, size_t streams_per_thread = 1);
/** \endinternal
 */
        HCRNGAPI hcrngStatus hcrngXorwowDeviceRandomNArray_single(hc::accelerator_view accl_view, size_t streamCount, hcrngXorwowStream *streams,
	       size_t numberCount, float mu, float sigma, float *outBuffer, int streamlength = 0, size_t streams_per_thread = 1);
        HCRNGAPI hcrngStatus hcrngXorwowDeviceRandomNArray_double(hc::accelerator_view accl_view, size_t streamCount, hcrngXorwowStream *streams,
	       size_t numberCount, double mu, double sigma, double *outBuffer, int streamlength = 0, size_t streams_per_thread = 1);
	/*! @copybrief hcrngWriteStreamInfo()
	*  @see hcrngWriteStreamInfo()
	*/
	HCRNGAPI hcrngStatus hcrngXorwowWriteStreamInfo(const hcrngXorwowStream* stream, FILE *file);

        HCRNGAPI hcrngStatus hcrngXorwowDeviceRandomUnsignedIntegerArray(hc::accelerator_view accl_view, size_t streamCount, hcrngXorwowStream *streams,
                size_t numberCount, unsigned int *outBuffer, int streamlength = 0, size_t streams_per_thread = 1);

#endif // XORWOW_H
