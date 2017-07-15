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

#include "include/hcRNG/box_muller_transform.h"
#include "include/hcRNG/hcRNG.h"
#include <hc.hpp>

#define BLOCK_SIZE 256

hcrngStatus box_muller_transform_single(hc::accelerator_view& accl_view,
                                        float mu, float sigma, float* Randomnum,
                                        size_t numberCount) {
  const float two_pi = 2.0 * 3.14159265358979323846;
  long size = (numberCount / 2 + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
  hc::extent<1> grdExt(size);
  hc::tiled_extent<1> t_ext(grdExt, BLOCK_SIZE);
  hc::parallel_for_each(accl_view, t_ext, [=](hc::tiled_index<1> tidx)[[hc]] {
    float z0, z1;
    int gid = tidx.global[0];
    if (gid < numberCount / 2) {
      z0 = hc::fast_math::sqrt(-2.0f * hc::fast_math::log(Randomnum[gid * 2])) *
           hc::fast_math::cos(two_pi * Randomnum[gid * 2 + 1]);
      z1 = hc::fast_math::sqrt(-2.0f * hc::fast_math::log(Randomnum[gid * 2])) *
           hc::fast_math::sin(two_pi * Randomnum[gid * 2 + 1]);
      Randomnum[gid * 2] = z0 * sigma + mu;
      Randomnum[gid * 2 + 1] = z1 * sigma + mu;
    }
  }).wait();
  return HCRNG_SUCCESS;
}

hcrngStatus box_muller_transform_double(hc::accelerator_view& accl_view,
                                        double mu, double sigma,
                                        double* Randomnum, size_t numberCount) {
  const double two_pi = 2.0 * 3.14159265358979323846;
  long size = (numberCount / 2 + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
  hc::extent<1> grdExt(size);
  hc::tiled_extent<1> t_ext(grdExt, BLOCK_SIZE);
  hc::parallel_for_each(accl_view, t_ext, [=](hc::tiled_index<1> tidx)[[hc]] {
    double z0, z1;
    int gid = tidx.global[0];
    if (gid < numberCount / 2) {
      z0 = hc::precise_math::sqrt(-2.0 * hc::precise_math::log(Randomnum[gid * 2])) *
           hc::precise_math::cos(two_pi * Randomnum[gid * 2 + 1]);
      z1 = hc::precise_math::sqrt(-2.0 * hc::precise_math::log(Randomnum[gid * 2])) *
           hc::precise_math::sin(two_pi * Randomnum[gid * 2 + 1]);
      Randomnum[gid * 2] = z0 * sigma + mu;
      Randomnum[gid * 2 + 1] = z1 * sigma + mu;
    }
  }).wait();
  return HCRNG_SUCCESS;
}
