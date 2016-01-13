#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <hcRNG/philox432.h>
#include <hcRNG/hcRNG.h>
#include <hc.hpp>
#include "gtest/gtest.h"
using namespace hc;

void multistream_fill_array(size_t spwi, size_t gsize, size_t quota, int substream_length, hcrngPhilox432Stream* streams, double* out_)
{
  for (size_t i = 0; i < quota; i++) {
      for (size_t gid = 0; gid < gsize; gid++) {
          hcrngPhilox432Stream* s = &streams[spwi * gid];
          double* out = &out_[spwi * (i * gsize + gid)];
          if ((i > 0) && (substream_length > 0) && (i % substream_length == 0))
              hcrngPhilox432ForwardToNextSubstreams(spwi, s);
          else if ((i > 0) && (substream_length < 0) && (i % (-substream_length) == 0))
              hcrngPhilox432RewindSubstreams(spwi, s);
          for (size_t sid = 0; sid < spwi; sid++) {
              out[sid] = hcrngPhilox432RandomU01(&s[sid]);
          }
      }
  }
}

TEST(philox432Double_test, Functional_check_philox432Double)
{
        hcrngPhilox432Stream* stream = NULL;
        hcrngStatus status = HCRNG_SUCCESS;
        bool ispassed1 = 1, ispassed2 = 1;
        size_t streamBufferSize;
        size_t streamCount = 10;
        size_t numberCount = 100;
        int stream_length = 5;
        size_t streams_per_thread = 2;
        double *Random1 = (double*) malloc(sizeof(double) * numberCount);
        double *Random2 = (double*) malloc(sizeof(double) * numberCount);
        hc::array_view<double> outBufferDevice(numberCount, Random1);
        hc::array_view<double> outBufferHost(numberCount, Random2);
        hcrngPhilox432Stream *streams = hcrngPhilox432CreateStreams(NULL, streamCount, &streamBufferSize, NULL);
        hc::array_view<hcrngPhilox432Stream> streams_buffer(streamCount, streams);
        status = hcrngPhilox432DeviceRandomU01Array_double(streamCount, streams_buffer, numberCount, outBufferDevice);
        EXPECT_EQ(status, 0);
        for (size_t i = 0; i < numberCount; i++)
            outBufferHost[i] = hcrngPhilox432RandomU01(&streams[i % streamCount]);   
        for(int i =0; i < numberCount; i++) {
           EXPECT_EQ(outBufferDevice[i], outBufferHost[i]);
        }
        double *Random3 = (double*) malloc(sizeof(double) * numberCount);
        double *Random4 = (double*) malloc(sizeof(double) * numberCount);
        hc::array_view<double> outBufferDevice_substream(numberCount, Random3);
        status = hcrngPhilox432DeviceRandomU01Array_double(streamCount, streams_buffer, numberCount, outBufferDevice_substream, stream_length, streams_per_thread);
        EXPECT_EQ(status, 0);
        multistream_fill_array(streams_per_thread, streamCount/streams_per_thread, numberCount/streamCount, stream_length, streams, Random4);
        for(int i =0; i < numberCount; i++) {
           EXPECT_EQ(outBufferDevice_substream[i], Random4[i]);
        }
}
