#include <hcRNG/hcRNG.h>
#include <hcRNG/mrg31k3p.h>
#include <hcRNG/mrg32k3a.h>
#include <hcRNG/lfsr113.h>
#include <hcRNG/philox432.h>
#include "gtest/gtest.h"
#define STREAM_COUNT 10
#define NUMBER_COUNT 100 /*Should be a multiple of streamCount */
#define SUBSTREAM_LENGTH_1 0 /* do not use substreams */
#define SUBSTREAM_LENGTH_2 5 /* go to next substreams after Substream_length values */
#define SUBSTREAM_LENGTH_3 -5 /* restart substream after Substream_length values */
#define STREAMS_PER_THREAD 2
typedef float fp_type;

TEST(hcrng_Randomarray, func_correct_Randomarray_single_Mrg31k3p ) {
     hcrngStatus status, err;
     size_t streamBufferSize;
     //Allocate Host pointers 
     fp_type *Random1 = (fp_type*) malloc(sizeof(fp_type) * NUMBER_COUNT);
     fp_type *Random2 = (fp_type*) malloc(sizeof(fp_type) * NUMBER_COUNT);

     //Allocate buffer for Device output
     hc::array_view<fp_type> outBuffer(NUMBER_COUNT, Random1);
     hcrngMrg31k3pStream *streams = hcrngMrg31k3pCreateStreams(NULL, STREAM_COUNT, &streamBufferSize, &err);
     EXPECT_EQ(err, HCRNG_SUCCESS);
     hc::array_view<hcrngMrg31k3pStream> streams_buffer(STREAM_COUNT, streams);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments)
     status = hcrngMrg31k3pDeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (SUBSTREAM_LENGTH = 0)
     status = hcrngMrg31k3pDeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer, SUBSTREAM_LENGTH_1, STREAMS_PER_THREAD);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (SUBSTREAM_LENGTH > 0)
     status = hcrngMrg31k3pDeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer, SUBSTREAM_LENGTH_2, STREAMS_PER_THREAD);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (SUBSTREAM_LENGTH < 0)
     status = hcrngMrg31k3pDeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer, SUBSTREAM_LENGTH_3, STREAMS_PER_THREAD);
     EXPECT_EQ(status, HCRNG_SUCCESS);
   
     //Invoke Random number generator function in Device (here last 2 arguments are default arguments and streamcount < 1)
     status = hcrngMrg31k3pDeviceRandomU01Array_single( 0 , streams_buffer, NUMBER_COUNT, outBuffer);
     EXPECT_EQ(status, HCRNG_INVALID_VALUE);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments and numbercount < 1)
     status = hcrngMrg31k3pDeviceRandomU01Array_single( STREAM_COUNT, streams_buffer, 0 , outBuffer);
     EXPECT_EQ(status, HCRNG_INVALID_VALUE);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments and numbercount is not a multiple of streamcount)
     status = hcrngMrg31k3pDeviceRandomU01Array_single( STREAM_COUNT, streams_buffer , NUMBER_COUNT + 1, outBuffer);
     EXPECT_EQ(status, HCRNG_INVALID_VALUE); 
}

TEST(hcrng_Randomarray, func_correct_Randomarray_single_Mrg32k3a ) {
     hcrngStatus status, err;
     size_t streamBufferSize;
     //Allocate Host pointers 
     fp_type *Random1 = (fp_type*) malloc(sizeof(fp_type) * NUMBER_COUNT);
     fp_type *Random2 = (fp_type*) malloc(sizeof(fp_type) * NUMBER_COUNT);

     //Allocate buffer for Device output
     hc::array_view<fp_type> outBuffer(NUMBER_COUNT, Random1);
     hcrngMrg32k3aStream *streams = hcrngMrg32k3aCreateStreams(NULL, STREAM_COUNT, &streamBufferSize, &err);
     EXPECT_EQ(err, HCRNG_SUCCESS);
     hc::array_view<hcrngMrg32k3aStream> streams_buffer(STREAM_COUNT, streams);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments)
     status = hcrngMrg32k3aDeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (SUBSTREAM_LENGTH = 0)
     status = hcrngMrg32k3aDeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer, SUBSTREAM_LENGTH_1, STREAMS_PER_THREAD);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (SUBSTREAM_LENGTH > 0)
     status = hcrngMrg32k3aDeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer, SUBSTREAM_LENGTH_2, STREAMS_PER_THREAD);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (SUBSTREAM_LENGTH < 0)
     status = hcrngMrg32k3aDeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer, SUBSTREAM_LENGTH_3, STREAMS_PER_THREAD);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments and streamcount < 1)
     status = hcrngMrg32k3aDeviceRandomU01Array_single( 0 , streams_buffer, NUMBER_COUNT, outBuffer);
     EXPECT_EQ(status, HCRNG_INVALID_VALUE);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments and numbercount < 1)
     status = hcrngMrg32k3aDeviceRandomU01Array_single( STREAM_COUNT, streams_buffer, 0 , outBuffer);
     EXPECT_EQ(status, HCRNG_INVALID_VALUE);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments and numbercount is not a multiple of streamcount)
     status = hcrngMrg32k3aDeviceRandomU01Array_single( STREAM_COUNT, streams_buffer , NUMBER_COUNT + 1, outBuffer);
     EXPECT_EQ(status, HCRNG_INVALID_VALUE);
}

TEST(hcrng_Randomarray, func_correct_Randomarray_single_Lfsr113 ) {
     hcrngStatus status, err;
     size_t streamBufferSize;
     //Allocate Host pointers
     fp_type *Random1 = (fp_type*) malloc(sizeof(fp_type) * NUMBER_COUNT);
     fp_type *Random2 = (fp_type*) malloc(sizeof(fp_type) * NUMBER_COUNT);

     //Allocate buffer for Device output
     hc::array_view<fp_type> outBuffer(NUMBER_COUNT, Random1);
     hcrngLfsr113Stream *streams = hcrngLfsr113CreateStreams(NULL, STREAM_COUNT, &streamBufferSize, &err);
     EXPECT_EQ(err, HCRNG_SUCCESS);
     hc::array_view<hcrngLfsr113Stream> streams_buffer(STREAM_COUNT, streams);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments)
     status = hcrngLfsr113DeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (SUBSTREAM_LENGTH = 0)
     status = hcrngLfsr113DeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer, SUBSTREAM_LENGTH_1, STREAMS_PER_THREAD);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (SUBSTREAM_LENGTH > 0)
     status = hcrngLfsr113DeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer, SUBSTREAM_LENGTH_2, STREAMS_PER_THREAD);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (SUBSTREAM_LENGTH < 0)
     status = hcrngLfsr113DeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer, SUBSTREAM_LENGTH_3, STREAMS_PER_THREAD);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments and streamcount < 1)
     status = hcrngLfsr113DeviceRandomU01Array_single( 0 , streams_buffer, NUMBER_COUNT, outBuffer);
     EXPECT_EQ(status, HCRNG_INVALID_VALUE);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments and numbercount < 1)
     status = hcrngLfsr113DeviceRandomU01Array_single( STREAM_COUNT, streams_buffer, 0 , outBuffer);
     EXPECT_EQ(status, HCRNG_INVALID_VALUE);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments and numbercount is not a multiple of streamcount)
     status = hcrngLfsr113DeviceRandomU01Array_single( STREAM_COUNT, streams_buffer , NUMBER_COUNT + 1, outBuffer);
     EXPECT_EQ(status, HCRNG_INVALID_VALUE);
}

TEST(hcrng_Randomarray, func_correct_Randomarray_single_Philox432 ) {
     hcrngStatus status, err;
     size_t streamBufferSize;
     //Allocate Host pointers
     fp_type *Random1 = (fp_type*) malloc(sizeof(fp_type) * NUMBER_COUNT);
     fp_type *Random2 = (fp_type*) malloc(sizeof(fp_type) * NUMBER_COUNT);

     //Allocate buffer for Device output
     hc::array_view<fp_type> outBuffer(NUMBER_COUNT, Random1);
     hcrngPhilox432Stream *streams = hcrngPhilox432CreateStreams(NULL, STREAM_COUNT, &streamBufferSize, &err);
     EXPECT_EQ(err, HCRNG_SUCCESS);
     hc::array_view<hcrngPhilox432Stream> streams_buffer(STREAM_COUNT, streams);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments)
     status = hcrngPhilox432DeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (SUBSTREAM_LENGTH = 0)
     status = hcrngPhilox432DeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer, SUBSTREAM_LENGTH_1, STREAMS_PER_THREAD);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (SUBSTREAM_LENGTH > 0)
     status = hcrngPhilox432DeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer, SUBSTREAM_LENGTH_2, STREAMS_PER_THREAD);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (SUBSTREAM_LENGTH < 0)
     status = hcrngPhilox432DeviceRandomU01Array_single(STREAM_COUNT, streams_buffer, NUMBER_COUNT, outBuffer, SUBSTREAM_LENGTH_3, STREAMS_PER_THREAD);
     EXPECT_EQ(status, HCRNG_SUCCESS);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments and streamcount < 1)
     status = hcrngPhilox432DeviceRandomU01Array_single( 0 , streams_buffer, NUMBER_COUNT, outBuffer);
     EXPECT_EQ(status, HCRNG_INVALID_VALUE);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments and numbercount < 1)
     status = hcrngPhilox432DeviceRandomU01Array_single( STREAM_COUNT, streams_buffer, 0 , outBuffer);
     EXPECT_EQ(status, HCRNG_INVALID_VALUE);

     //Invoke Random number generator function in Device (here last 2 arguments are default arguments and numbercount is not a multiple of streamcount)
     status = hcrngPhilox432DeviceRandomU01Array_single( STREAM_COUNT, streams_buffer , NUMBER_COUNT + 1, outBuffer);
     EXPECT_EQ(status, HCRNG_INVALID_VALUE);
}

