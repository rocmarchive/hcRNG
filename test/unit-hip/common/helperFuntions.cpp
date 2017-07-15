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

#include "../gtest/gtest.h"
// HIP runtime
#include <hip/hip_runtime_api.h>
#include "hiprng.h"
// HIP and hcrng functions
#include <helper_functions.h>

TEST(hiprng_common, hip_SetStream_Mrg31k3p) {
  hiprngGenerator_t gen;
  hiprngStatus_t status;

  //  Create pseudo-random number generator
  status = hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_MRG31K3P);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  status = hiprngSetStream(gen, hipStreamDefault);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hiprngDestroyGenerator(gen);
}

TEST(hiprng_common, hip_SetStream_Mrg32k3a) {
  hiprngGenerator_t gen;
  hiprngStatus_t status;

  //  Create pseudo-random number generator
  status = hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_MRG32K3A);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  status = hiprngSetStream(gen, hipStreamDefault);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hiprngDestroyGenerator(gen);
}

TEST(hiprng_common, hip_SetStream_Lfsr113) {
  hiprngGenerator_t gen;
  hiprngStatus_t status;

  //  Create pseudo-random number generator
  status = hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_LFSR113);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  status = hiprngSetStream(gen, hipStreamDefault);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hiprngDestroyGenerator(gen);
}

TEST(hiprng_common, hip_SetStream_Philox432) {
  hiprngGenerator_t gen;
  hiprngStatus_t status;

  //  Create pseudo-random number generator
  status = hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_PHILOX432);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  status = hiprngSetStream(gen, hipStreamDefault);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hiprngDestroyGenerator(gen);
}
