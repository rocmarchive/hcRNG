/*! @file mrg31k3p.c.h
 *  @brief Code for the MRG31k3p generator common to the host and device
 */

#pragma once
#ifndef PRIVATE_MRG31K3P_CH
#define PRIVATE_MRG31K3P_CH

#define mrg31k3p_M1 2147483647             /* 2^31 - 1 */
#define mrg31k3p_M2 2147462579             /* 2^31 - 21069 */

#define mrg31k3p_MASK12 511                /* 2^9 - 1 */
#define mrg31k3p_MASK13 16777215           /* 2^24 - 1 */
#define mrg31k3p_MASK2 65535               /* 2^16 - 1 */
#define mrg31k3p_MULT2 21069

#define mrg31k3p_NORM_double 4.656612873077392578125e-10  /* 1/2^31 */
#define mrg31k3p_NORM_float  4.6566126e-10
// hcrngMrg31k3p_A1p72 and hcrngMrg31k3p_A2p72 jump 2^72 steps forward
static
unsigned int hcrngMrg31k3p_A1p72[3][3] = { 
    {1516919229,  758510237, 499121365},
    {1884998244, 1516919229, 335398200},
    {601897748,  1884998244, 358115744}
};

static
unsigned int hcrngMrg31k3p_A2p72[3][3] = { 
    {1228857673, 1496414766,  954677935},
    {1133297478, 1407477216, 1496414766},
    {2002613992, 1639496704, 1407477216}
};


hcrngStatus hcrngMrg31k3pCopyOverStreams(size_t count, hcrngMrg31k3pStream* destStreams, const hcrngMrg31k3pStream* srcStreams) restrict (amp)
{
    //Check params
	if (!destStreams)
	return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): destStreams cannot be NULL", __func__);
	if (!srcStreams)
	return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): srcStreams cannot be NULL", __func__);

    for (size_t i = 0; i < count; i++)
		destStreams[i] = srcStreams[i];

    return HCRNG_SUCCESS;
}

/*! @brief Advance the rng one step and returns z such that 1 <= z <= mrg31k3p_M1
 */
static unsigned int hcrngMrg31k3pNextState(hcrngMrg31k3pStreamState* currentState) restrict (amp)
{
	
	unsigned int* g1 = currentState->g1;
	unsigned int* g2 = currentState->g2;
	unsigned int y1, y2;

	// first component
	y1 = ((g1[1] & mrg31k3p_MASK12) << 22) + (g1[1] >> 9)
		+ ((g1[2] & mrg31k3p_MASK13) << 7) + (g1[2] >> 24);

	if (y1 >= mrg31k3p_M1)
		y1 -= mrg31k3p_M1;

	y1 += g1[2];
	if (y1 >= mrg31k3p_M1)
		y1 -= mrg31k3p_M1;

	g1[2] = g1[1];
	g1[1] = g1[0];
	g1[0] = y1;

	// second component
	y1 = ((g2[0] & mrg31k3p_MASK2) << 15) + (mrg31k3p_MULT2 * (g2[0] >> 16));
	if (y1 >= mrg31k3p_M2)
		y1 -= mrg31k3p_M2;
	y2 = ((g2[2] & mrg31k3p_MASK2) << 15) + (mrg31k3p_MULT2 * (g2[2] >> 16));
	if (y2 >= mrg31k3p_M2)
		y2 -= mrg31k3p_M2;
	y2 += g2[2];
	if (y2 >= mrg31k3p_M2)
		y2 -= mrg31k3p_M2;
	y2 += y1;
	if (y2 >= mrg31k3p_M2)
		y2 -= mrg31k3p_M2;

	g2[2] = g2[1];
	g2[1] = g2[0];
	g2[0] = y2;

	if (g1[0] <= g2[0])
		return (g1[0] - g2[0] + mrg31k3p_M1);
	else
		return (g1[0] - g2[0]);
}

// We use an underscore on the r.h.s. to avoid potential recursion with certain
// preprocessors.
#define IMPLEMENT_GENERATE_FOR_TYPE(fptype) \
	\
	fptype hcrngMrg31k3pRandomU01_##fptype(hcrngMrg31k3pStream* stream) restrict (amp) { \
	    return hcrngMrg31k3pNextState(&stream->current) * mrg31k3p_NORM_##fptype; \
	} \
	\
	int hcrngMrg31k3pRandomInteger_##fptype(hcrngMrg31k3pStream* stream, int i, int j) restrict (amp) { \
	    return i + (int)((j - i + 1) * hcrngMrg31k3pRandomU01_##fptype(stream)); \
	} \
	\
	hcrngStatus hcrngMrg31k3pRandomU01Array_##fptype(hcrngMrg31k3pStream* stream, size_t count, fptype* buffer) restrict (amp) { \
		if (!stream) \
			return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): stream cannot be NULL", __func__); \
		if (!buffer) \
			return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): buffer cannot be NULL", __func__); \
		for (size_t i = 0; i < count; i++)  \
			buffer[i] = hcrngMrg31k3pRandomU01_##fptype(stream); \
		return HCRNG_SUCCESS; \
	} \
	\
	hcrngStatus hcrngMrg31k3pRandomIntegerArray_##fptype(hcrngMrg31k3pStream* stream, int i, int j, size_t count, int* buffer) restrict (amp) { \
		if (!stream) \
			return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): stream cannot be NULL", __func__); \
		if (!buffer) \
			return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): buffer cannot be NULL", __func__); \
		for (size_t k = 0; k < count; k++) \
			buffer[k] = hcrngMrg31k3pRandomInteger_##fptype(stream, i, j); \
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



hcrngStatus hcrngMrg31k3pRewindStreams(size_t count, hcrngMrg31k3pStream* streams) restrict (amp)
{
	//Check params
	if (!streams)
		return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): streams cannot be NULL", __func__);
	//Reset current state to the stream initial state
	for (size_t j = 0; j < count; j++) {
		streams[j].current = streams[j].substream = streams[j].initial;
	}

	return HCRNG_SUCCESS;
}

hcrngStatus hcrngMrg31k3pRewindSubstreams(size_t count, hcrngMrg31k3pStream* streams) restrict (amp)
{
	//Check params
	if (!streams)
		return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): streams cannot be NULL", __func__);
	//Reset current state to the subStream initial state
	for (size_t j = 0; j < count; j++) {
		streams[j].current = streams[j].substream;
	}

	return HCRNG_SUCCESS;
}

hcrngStatus hcrngMrg31k3pForwardToNextSubstreams(size_t count, hcrngMrg31k3pStream* streams) restrict (amp)
{
	//Check params
	if (!streams)
		return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): streams cannot be NULL", __func__);
	
	for (size_t k = 0; k < count; k++) {
		modMatVec (hcrngMrg31k3p_A1p72, streams[k].substream.g1, streams[k].substream.g1, mrg31k3p_M1);
		modMatVec (hcrngMrg31k3p_A2p72, streams[k].substream.g2, streams[k].substream.g2, mrg31k3p_M2);
		streams[k].current = streams[k].substream;
	}

	return HCRNG_SUCCESS;
}

hcrngStatus hcrngMrg31k3pMakeOverSubstreams(hcrngMrg31k3pStream* stream, size_t count, hcrngMrg31k3pStream* substreams) restrict (amp)
{
	for (size_t i = 0; i < count; i++) {
		hcrngStatus err;
		// snapshot current stream into substreams[i]
		err = hcrngMrg31k3pCopyOverStreams(1, &substreams[i], stream);
		if (err != HCRNG_SUCCESS)
		    return err;
		// advance to next substream
		err = hcrngMrg31k3pForwardToNextSubstreams(1, stream);
		if (err != HCRNG_SUCCESS)
		    return err;
	}
	return HCRNG_SUCCESS;
}

hcrngStatus hcrngMrg31k3pCopyOverStreamsFromGlobal(size_t count, hcrngMrg31k3pStream* destStreams, hcrngMrg31k3pStream* srcStreams) restrict (amp)
{
    //Check params
    if (!destStreams)
        return hcrngSetErrorString(HCRNG_INVALID_VALUE, "hcrngMrg31k3pCopyOverStreamsFromGlobal(): destStreams cannot be NULL");
    if (!srcStreams)
        return hcrngSetErrorString(HCRNG_INVALID_VALUE, "hcrngMrg31k3pCopyOverStreamsFromGlobal(): srcStreams cannot be NULL");

    for (size_t i = 0; i < count; i++) {
        destStreams[i].current   = srcStreams[i].current;
        destStreams[i].initial   = srcStreams[i].initial;
#ifdef CLRNG_ENABLE_SUBSTREAMS
        destStreams[i].substream = srcStreams[i].substream;
#endif
    }

    return HCRNG_SUCCESS;
}

hcrngStatus hcrngMrg31k3pCopyOverStreamsToGlobal(size_t count, hcrngMrg31k3pStream* destStreams, hcrngMrg31k3pStream* srcStreams) restrict (amp)
{
    //Check params
    if (!destStreams)
        return hcrngSetErrorString(HCRNG_INVALID_VALUE, "hcrngMrg31k3pCopyOverStreamsToGlobal(): destStreams cannot be NULL");
    if (!srcStreams)
        return hcrngSetErrorString(HCRNG_INVALID_VALUE, "hcrngMrg31k3pCopyOverStreamsToGlobal(): srcStreams cannot be NULL");

    for (size_t i = 0; i < count; i++) {
        destStreams[i].current   = srcStreams[i].current;
        destStreams[i].initial   = srcStreams[i].initial;
#ifdef CLRNG_ENABLE_SUBSTREAMS
        destStreams[i].substream = srcStreams[i].substream;
#endif
    }

    return HCRNG_SUCCESS;
}

#endif // PRIVATE_MRG31K3P_CH
