#include <hcRNG/hcRNG.h>
#include <hcRNG/mrg31k3p.h>
#include <hcRNG/mrg32k3a.h>
#include <hcRNG/lfsr113.h>
#include <hcRNG/philox432.h>
#include "gtest/gtest.h"
#define STREAM_COUNT 10

TEST(hcrng_Streams, func_correct_Streams_Mrg31k3p ) {
    hcrngMrg31k3pStream* stream1 = NULL;
    hcrngStatus status, err; 
    status = hcrngMrg31k3pDestroyStreams(stream1);
    EXPECT_EQ(status, HCRNG_SUCCESS);
    size_t streamBufferSize;
    hcrngMrg31k3pStream *stream2 = hcrngMrg31k3pCreateStreams(NULL, STREAM_COUNT, &streamBufferSize, &err);
    EXPECT_EQ(err, HCRNG_SUCCESS);
    status = hcrngMrg31k3pDestroyStreams(stream2);
    EXPECT_EQ(status, HCRNG_SUCCESS);
}

TEST(hcrng_Streams, func_correct_Streams_Mrg32k3a ) {
    hcrngMrg32k3aStream* stream1 = NULL;
    hcrngStatus status, err;
    status = hcrngMrg32k3aDestroyStreams(stream1);
    EXPECT_EQ(status, HCRNG_SUCCESS);
    size_t streamBufferSize;
    hcrngMrg32k3aStream *stream2 = hcrngMrg32k3aCreateStreams(NULL, STREAM_COUNT, &streamBufferSize, &err);
    EXPECT_EQ(err, HCRNG_SUCCESS);
    status = hcrngMrg32k3aDestroyStreams(stream2);
    EXPECT_EQ(status, HCRNG_SUCCESS);
}

TEST(hcrng_Streams, func_correct_Streams_Lfsr113 ) {
    hcrngLfsr113Stream* stream1 = NULL;
    hcrngStatus status, err;
    status = hcrngLfsr113DestroyStreams(stream1);
    EXPECT_EQ(status, HCRNG_SUCCESS);
    size_t streamBufferSize;
    hcrngLfsr113Stream *stream2 = hcrngLfsr113CreateStreams(NULL, STREAM_COUNT, &streamBufferSize, &err);
    EXPECT_EQ(err, HCRNG_SUCCESS);
    status = hcrngLfsr113DestroyStreams(stream2);
    EXPECT_EQ(status, HCRNG_SUCCESS);
}

TEST(hcrng_Streams, func_correct_Streams_Philox432 ) {
    hcrngPhilox432Stream* stream1 = NULL;
    hcrngStatus status, err;
    status = hcrngPhilox432DestroyStreams(stream1);
    EXPECT_EQ(status, HCRNG_SUCCESS);
    size_t streamBufferSize;
    hcrngPhilox432Stream *stream2 = hcrngPhilox432CreateStreams(NULL, STREAM_COUNT, &streamBufferSize, &err);
    EXPECT_EQ(err, HCRNG_SUCCESS);
    status = hcrngPhilox432DestroyStreams(stream2);
    EXPECT_EQ(status, HCRNG_SUCCESS);
}
