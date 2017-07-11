#include "hcRNG/xorwow.h"
#include "hcRNG/box_muller_transform.h"
#define BLOCK_SIZE 256
#define MODULAR_NUMBER_TYPE unsigned long
#define MODULAR_FIXED_SIZE 3
#include "../include/hcRNG/private/modular.c.h"

// code that is common to host and device
#include "../include/hcRNG/private/xorwow.c.h"

hcrngStatus validateSeed(const hcrngXorwowStreamState* baseState)
{
  return HCRNG_SUCCESS;
}

hcrngStatus hcrngXorwowSetAcclView(hcrngXorwowStreamCreator* creator, hc::accelerator_view accl_view, void* stream) {
  creator->currentAcclView = &accl_view;
  creator->currentStream = stream;
  return HCRNG_SUCCESS;
}

hcrngStatus hcrngXorwowGetAcclView(hcrngXorwowStreamCreator* creator, hc::accelerator_view *&accl_view, void** stream) {
  if (creator == nullptr) {
    return HCRNG_INVALID_STREAM_CREATOR;
  }
  accl_view = creator->currentAcclView;
  stream = &(creator->currentStream);
  return HCRNG_SUCCESS;
}

hcrngStatus hcrngXorwowDestroyStreamCreator(hcrngXorwowStreamCreator* creator)
{
	if (creator != NULL)
		free(creator);
	return HCRNG_SUCCESS;
}

hcrngStatus hcrngXorwowSetBaseCreatorState(hcrngXorwowStreamCreator* creator, const hcrngXorwowStreamState* baseState)
{
	//Check params
	if (creator == NULL)
		return hcrngSetErrorString(HCRNG_INVALID_STREAM_CREATOR, "%s(): modifying the default stream creator is forbidden", __func__);
	if (baseState == NULL)
		return hcrngSetErrorString(HCRNG_INVALID_VALUE, "%s(): baseState cannot be NULL", __func__);

	hcrngStatus err = validateSeed(baseState);

	if (err == HCRNG_SUCCESS) {
		// initialize new creator
		creator->initialState = creator->nextState = *baseState;
	}

	return err;
}

hcrngXorwowStream* hcrngXorwowAllocStreams(size_t count, size_t* bufSize, hcrngStatus* err)
{
	hcrngStatus err_ = HCRNG_SUCCESS;
	size_t bufSize_ = count * sizeof(hcrngXorwowStream);


	hcrngXorwowStream* buf = (hcrngXorwowStream*)malloc(bufSize_);

	if (buf == NULL) {
		// allocation failed
		err_ = hcrngSetErrorString(HCRNG_OUT_OF_RESOURCES, "%s(): could not allocate memory for streams", __func__);
		bufSize_ = 0;
	}

	// set buffer size if needed
	if (bufSize != NULL)
		*bufSize = bufSize_;

	// set error status if needed
	if (err != NULL)
		*err = err_;

	return buf;
}

hcrngStatus hcrngXorwowDestroyStreams(hcrngXorwowStream* streams)
{
	if (streams != NULL)
		free(streams);
	return HCRNG_SUCCESS;
}

void XorwowAdvanceStream(hcrngXorwowStreamState* s)
{
    unsigned long t;
    s->v[0] += s->d;
    t = (s->v[0] ^ (s->v[0] >> 2));
    s->v[0] = s->v[1];
    s->v[1] = s->v[2];
    s->v[2] = s->v[3];
    s->v[3] = s->v[4];
    s->v[4] = (s->v[4] ^ (s->v[4] <<4)) ^ (t ^ (t << 1));
    s->d += 362437;
    return;
}

static hcrngStatus XorwowCreateStream(hcrngXorwowStreamCreator* creator, hcrngXorwowStream* buffer)
{
	//Check params
	if (buffer == NULL)
		return HCRNG_INVALID_VALUE;

	// use default creator if not given
	if (creator == NULL)
		creator = &defaultStreamCreator_Xorwow;

	// initialize stream
	buffer->current = buffer->initial = buffer->substream = creator->nextState;

        XorwowAdvanceStream(&creator->nextState); 

	return HCRNG_SUCCESS;
}

hcrngStatus hcrngXorwowCreateOverStreams(hcrngXorwowStreamCreator* creator, size_t count, hcrngXorwowStream* streams)
{
	// iterate over all individual stream buffers
	for (size_t i = 0; i < count; i++) {

		hcrngStatus err = XorwowCreateStream(creator, &streams[i]);

		// abort on error
		if (err != HCRNG_SUCCESS)
			return err;
	}

	return HCRNG_SUCCESS;
}

hcrngXorwowStream* hcrngXorwowCreateStreams(hcrngXorwowStreamCreator* creator, size_t count, size_t* bufSize, hcrngStatus* err)
{
	hcrngStatus err_;
	size_t bufSize_;
	hcrngXorwowStream* streams = hcrngXorwowAllocStreams(count, &bufSize_, &err_);

	if (err_ == HCRNG_SUCCESS)
		err_ = hcrngXorwowCreateOverStreams(creator, count, streams);

	if (bufSize != NULL)
		*bufSize = bufSize_;

	if (err != NULL)
		*err = err_;

	return streams;
}

hcrngStatus hcrngXorwowDeviceRandomUnsignedIntegerArray(hc::accelerator_view accl_view, size_t streamCount, hcrngXorwowStream *streams,
        size_t numberCount, unsigned int *outBuffer, int streamlength, size_t streams_per_thread)
{
        if (streamCount < 1)
                return HCRNG_INVALID_VALUE;
        if (numberCount < 1)
                return HCRNG_INVALID_VALUE;
        hcrngStatus status = HCRNG_SUCCESS;
        long size = ((streamCount/streams_per_thread) + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
        hc::extent<1> grdExt(size);
        hc::tiled_extent<1> t_ext(grdExt, BLOCK_SIZE);
        hc::parallel_for_each(accl_view, t_ext, [ = ] (hc::tiled_index<1> tidx) [[hc, cpu]] {
           int gid = tidx.global[0];
           if(gid < streamCount/streams_per_thread) {
           for(int i =0; i < (numberCount-1)/streamCount+1; i++) {
              for (int j = 0; j < streams_per_thread; j++)
               if ((streams_per_thread * (i * (streamCount/streams_per_thread) + gid) + j) < numberCount)
                outBuffer[streams_per_thread * (i * (streamCount/streams_per_thread) + gid) + j] = hcrngXorwowRandomUnsignedInteger(&streams[streams_per_thread * gid + j]);
              }
           }
        }).wait();
        return status;
}
