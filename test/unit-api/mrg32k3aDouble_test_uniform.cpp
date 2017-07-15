/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "gtest/gtest.h"
#include <hc.hpp>
#include "include/hcRNG/hcRNG.h"
#include "include/hcRNG/mrg32k3a.h"
#include <hc_am.hpp>

void multistream_fill_array_uniform(size_t spwi, size_t gsize, size_t quota,
                                    int substream_length,
                                    hcrngMrg32k3aStream *streams,
                                    double *out_) {
  for (size_t i = 0; i < quota; i++) {
    for (size_t gid = 0; gid < gsize; gid++) {
      hcrngMrg32k3aStream *s = &streams[spwi * gid];
      double *out = &out_[spwi * (i * gsize + gid)];
      if ((i > 0) && (substream_length > 0) && (i % substream_length == 0))
        hcrngMrg32k3aForwardToNextSubstreams(spwi, s);
      else if ((i > 0) && (substream_length < 0) &&
               (i % (-substream_length) == 0))
        hcrngMrg32k3aRewindSubstreams(spwi, s);
      for (size_t sid = 0; sid < spwi; sid++) {
        out[sid] = hcrngMrg32k3aRandomU01(&s[sid]);
      }
    }
  }
}

TEST(mrg32k3aDouble_test_uniform, Functional_check_mrg32k3aDouble_uniform) {
  hcrngMrg32k3aStream *stream = NULL;
  hcrngStatus status = HCRNG_SUCCESS;
  bool ispassed1 = 1, ispassed2 = 1;
  size_t streamBufferSize;
  size_t NbrStreams = 1;
  size_t streamCount = 10;
  size_t numberCount = 100;
  int stream_length = 5;
  size_t streams_per_thread = 2;
  double *Random1 = (double *)malloc(sizeof(double) * numberCount);
  double *Random2 = (double *)malloc(sizeof(double) * numberCount);
  std::vector<hc::accelerator> acc = hc::accelerator::get_all();
  hc::accelerator_view accl_view = (acc[1].get_default_view());
  double *outBufferDevice =
      hc::am_alloc(sizeof(double) * numberCount, acc[1], 0);
  hcrngMrg32k3aStream *streams =
      hcrngMrg32k3aCreateStreams(NULL, streamCount, &streamBufferSize, NULL);
  hcrngMrg32k3aStream *streams_buffer =
      hc::am_alloc(sizeof(hcrngMrg32k3aStream) * streamCount, acc[1], 0);
  accl_view.copy(streams, streams_buffer,
                 streamCount * sizeof(hcrngMrg32k3aStream));
  status = hcrngMrg32k3aDeviceRandomU01Array_double(
      accl_view, streamCount, streams_buffer, numberCount, outBufferDevice);
  EXPECT_EQ(status, 0);
  accl_view.copy(outBufferDevice, Random1, numberCount * sizeof(double));
  for (size_t i = 0; i < numberCount; i++)
    Random2[i] = hcrngMrg32k3aRandomU01(&streams[i % streamCount]);
  for (int i = 0; i < numberCount; i++) {
    EXPECT_EQ(Random1[i], Random2[i]);
  }
  double *Random3 = (double *)malloc(sizeof(double) * numberCount);
  double *Random4 = (double *)malloc(sizeof(double) * numberCount);
  double *outBufferDevice_substream =
      hc::am_alloc(sizeof(double) * numberCount, acc[1], 0);
  status = hcrngMrg32k3aDeviceRandomU01Array_double(
      accl_view, streamCount, streams_buffer, numberCount,
      outBufferDevice_substream, stream_length, streams_per_thread);
  EXPECT_EQ(status, 0);
  accl_view.copy(outBufferDevice_substream, Random3,
                 numberCount * sizeof(double));
  multistream_fill_array_uniform(
      streams_per_thread, streamCount / streams_per_thread,
      numberCount / streamCount, stream_length, streams, Random4);
  for (int i = 0; i < numberCount; i++) {
    EXPECT_EQ(Random3[i], Random4[i]);
  }
}

