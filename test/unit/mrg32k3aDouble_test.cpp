#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <hcRNG/mrg32k3a.h>
#include <hcRNG/hcRNG.h>
#include <hc.hpp>
#include "gtest/gtest.h"
using namespace hc;

void multistream_fill_array(size_t spwi, size_t gsize, size_t quota, int substream_length, hcrngMrg32k3aStream* streams, double* out_)
{
  for (size_t i = 0; i < quota; i++) {
      for (size_t gid = 0; gid < gsize; gid++) {
          hcrngMrg32k3aStream* s = &streams[spwi * gid];
          double* out = &out_[spwi * (i * gsize + gid)];
          if ((i > 0) && (substream_length > 0) && (i % substream_length == 0))
              hcrngMrg32k3aForwardToNextSubstreams(spwi, s);
          else if ((i > 0) && (substream_length < 0) && (i % (-substream_length) == 0))
              hcrngMrg32k3aRewindSubstreams(spwi, s);
          for (size_t sid = 0; sid < spwi; sid++) {
              out[sid] = hcrngMrg32k3aRandomU01(&s[sid]);
          }
      }
  }
}

TEST(mrg32k3aDouble_test, Functional_check_mrg32k3aDouble)
{
        hcrngMrg32k3aStream* stream = NULL;
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
        hcrngMrg32k3aStream *streams = hcrngMrg32k3aCreateStreams(NULL, streamCount, &streamBufferSize, NULL);
        hc::array_view<hcrngMrg32k3aStream> streams_buffer(streamCount, streams);
        status = hcrngMrg32k3aDeviceRandomU01Array_double(streamCount, streams_buffer, numberCount, outBufferDevice);
        EXPECT_EQ(status, 0);
        for (size_t i = 0; i < numberCount; i++)
            outBufferHost[i] = hcrngMrg32k3aRandomU01(&streams[i % streamCount]);
        for(int i =0; i < numberCount; i++) {
           EXPECT_EQ(outBufferDevice[i],  outBufferHost[i]);
        }
        double *Random3 = (double*) malloc(sizeof(double) * numberCount);
        double *Random4 = (double*) malloc(sizeof(double) * numberCount);
        hc::array_view<double> outBufferDevice_substream(numberCount, Random3);
        status = hcrngMrg32k3aDeviceRandomU01Array_double(streamCount, streams_buffer, numberCount, outBufferDevice_substream, stream_length, streams_per_thread);
        if(status) std::cout << "TEST FAILED" << std::endl;
        multistream_fill_array(streams_per_thread, streamCount/streams_per_thread, numberCount/streamCount, stream_length, streams, Random4);
        for(int i =0; i < numberCount; i++) {
           EXPECT_EQ(outBufferDevice_substream[i],  Random4[i]); 
        }
}
