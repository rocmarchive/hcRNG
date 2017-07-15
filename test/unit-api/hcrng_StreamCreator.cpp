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

#include "include/hcRNG/hcRNG.h"
#include "include/hcRNG/lfsr113.h"
#include "include/hcRNG/mrg31k3p.h"
#include "include/hcRNG/mrg32k3a.h"
#include "include/hcRNG/philox432.h"
#include "gtest/gtest.h"

TEST(hcrng_Stream_Creator, Return_Check_Stream_Creator_Mrg31k3p) {
  hcrngMrg31k3pStreamCreator* creator1 = NULL;
  hcrngStatus status, err;

  /* Rewind Stream Creator with NULL creator */
  status = hcrngMrg31k3pRewindStreamCreator(creator1);
  EXPECT_EQ(status, HCRNG_SUCCESS);

  /* Destroy stream creator with NULL creator */
  status = hcrngMrg31k3pDestroyStreamCreator(creator1);
  EXPECT_EQ(status, HCRNG_SUCCESS);

  /* Create a creator using copy stream creator */
  hcrngMrg31k3pStreamCreator* creator2 =
      hcrngMrg31k3pCopyStreamCreator(NULL, &err);
  EXPECT_EQ(err, HCRNG_SUCCESS);

  /*Rewind and destroy allocated stream creator */
  status = hcrngMrg31k3pRewindStreamCreator(creator2);
  EXPECT_EQ(status, HCRNG_SUCCESS);
  status = hcrngMrg31k3pDestroyStreamCreator(creator2);
  EXPECT_EQ(status, HCRNG_SUCCESS);
}

TEST(hcrng_Stream_Creator, Return_Check_Stream_Creator_Mrg32k3a) {
  hcrngMrg32k3aStreamCreator* creator1 = NULL;
  hcrngStatus status, err;

  /* Rewind Stream Creator with NULL creator */
  status = hcrngMrg32k3aRewindStreamCreator(creator1);
  EXPECT_EQ(status, HCRNG_SUCCESS);

  /* Destroy stream creator with NULL creator */
  status = hcrngMrg32k3aDestroyStreamCreator(creator1);
  EXPECT_EQ(status, HCRNG_SUCCESS);

  /* Create a creator using copy stream creator */
  hcrngMrg32k3aStreamCreator* creator2 =
      hcrngMrg32k3aCopyStreamCreator(NULL, &err);
  EXPECT_EQ(err, HCRNG_SUCCESS);

  /*Rewind and destroy allocated stream creator */
  status = hcrngMrg32k3aRewindStreamCreator(creator2);
  EXPECT_EQ(status, HCRNG_SUCCESS);
  status = hcrngMrg32k3aDestroyStreamCreator(creator2);
  EXPECT_EQ(status, HCRNG_SUCCESS);
}

TEST(hcrng_Stream_Creator, Return_Check_Stream_Creator_Lfsr113) {
  hcrngLfsr113StreamCreator* creator1 = NULL;
  hcrngStatus status, err;

  /* Rewind Stream Creator with NULL creator */
  status = hcrngLfsr113RewindStreamCreator(creator1);
  EXPECT_EQ(status, HCRNG_SUCCESS);

  /* Destroy stream creator with NULL creator */
  status = hcrngLfsr113DestroyStreamCreator(creator1);
  EXPECT_EQ(status, HCRNG_SUCCESS);

  /* Create a creator using copy stream creator */
  hcrngLfsr113StreamCreator* creator2 =
      hcrngLfsr113CopyStreamCreator(NULL, &err);
  EXPECT_EQ(err, HCRNG_SUCCESS);

  /*Rewind and destroy allocated stream creator */
  status = hcrngLfsr113RewindStreamCreator(creator2);
  EXPECT_EQ(status, HCRNG_SUCCESS);
  status = hcrngLfsr113DestroyStreamCreator(creator2);
  EXPECT_EQ(status, HCRNG_SUCCESS);
}

TEST(hcrng_Stream_Creator, Return_Check_Stream_Creator_Philox432) {
  hcrngPhilox432StreamCreator* creator1 = NULL;
  hcrngStatus status, err;

  /* Rewind Stream Creator with NULL creator */
  status = hcrngPhilox432RewindStreamCreator(creator1);
  EXPECT_EQ(status, HCRNG_SUCCESS);

  /* Destroy stream creator with NULL creator */
  status = hcrngPhilox432DestroyStreamCreator(creator1);
  EXPECT_EQ(status, HCRNG_SUCCESS);

  /* Create a creator using copy stream creator */
  hcrngPhilox432StreamCreator* creator2 =
      hcrngPhilox432CopyStreamCreator(NULL, &err);
  EXPECT_EQ(err, HCRNG_SUCCESS);

  /*Rewind and destroy allocated stream creator */
  status = hcrngPhilox432RewindStreamCreator(creator2);
  EXPECT_EQ(status, HCRNG_SUCCESS);
  status = hcrngPhilox432DestroyStreamCreator(creator2);
  EXPECT_EQ(status, HCRNG_SUCCESS);
}
