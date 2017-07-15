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
#include "include/hcRNG/mrg31k3p.h"
#include "include/hcRNG/mrg32k3a.h"
#include "include/hcRNG/lfsr113.h"
#include "include/hcRNG/philox432.h"
#include "gtest/gtest.h"
#include <hc_am.hpp>

TEST(hcrng_SetGetAcclView, Return_Check_SetGetAcclView_Mrg31k3p ) {
    hcrngMrg31k3pStreamCreator* creator = NULL;
    creator = (hcrngMrg31k3pStreamCreator*)malloc(sizeof(hcrngMrg31k3pStreamCreator));
    *(hcrngMrg31k3pStreamCreator*)creator = defaultStreamCreator_Mrg31k3p;
    hcrngStatus status;

    void **stream = NULL;
    hc::accelerator default_acc;
    hc::accelerator_view av = default_acc.get_default_view();
    hc::accelerator_view default_acc_view = default_acc.get_default_view();
    hc::accelerator_view* accl_view = NULL;

    status = hcrngMrg31k3pSetAcclView(creator, default_acc_view);
    EXPECT_EQ(status, HCRNG_SUCCESS);
    // Now Get the Accl_view
    status = hcrngMrg31k3pGetAcclView(creator, accl_view, stream);
    EXPECT_EQ(status, HCRNG_SUCCESS);

    EXPECT_TRUE(accl_view != NULL);
    if (default_acc_view == *accl_view) {
        EXPECT_EQ(status, HCRNG_SUCCESS);
    }
    // We must expect the accl_view obtained is what that's being set
    EXPECT_EQ(default_acc_view, *accl_view);
}

TEST(hcrng_SetGetAcclView, Return_Check_SetGetAcclView_Mrg32k3a ) {
    hcrngMrg32k3aStreamCreator* creator = NULL;
    creator = (hcrngMrg32k3aStreamCreator*)malloc(sizeof(hcrngMrg32k3aStreamCreator));
    *(hcrngMrg32k3aStreamCreator*)creator = defaultStreamCreator_Mrg32k3a;
    hcrngStatus status;

    void **stream = NULL;
    hc::accelerator default_acc;
    hc::accelerator_view av = default_acc.get_default_view();
    hc::accelerator_view default_acc_view = default_acc.get_default_view();
    hc::accelerator_view* accl_view = NULL;

    status = hcrngMrg32k3aSetAcclView(creator, default_acc_view);
    EXPECT_EQ(status, HCRNG_SUCCESS);
    // Now Get the Accl_view
    status = hcrngMrg32k3aGetAcclView(creator, accl_view, stream);
    EXPECT_EQ(status, HCRNG_SUCCESS);

    EXPECT_TRUE(accl_view != NULL);
    if (default_acc_view == *accl_view) {
        EXPECT_EQ(status, HCRNG_SUCCESS);
    }
    // We must expect the accl_view obtained is what that's being set
    EXPECT_EQ(default_acc_view, *accl_view);
}

TEST(hcrng_SetGetAcclView, Return_Check_SetGetAcclView_Lfsr113 ) {
    hcrngLfsr113StreamCreator* creator = NULL;
    creator = (hcrngLfsr113StreamCreator*)malloc(sizeof(hcrngLfsr113StreamCreator));
    *(hcrngLfsr113StreamCreator*)creator = defaultStreamCreator_Lfsr113;
    hcrngStatus status;

    void **stream = NULL;
    hc::accelerator default_acc;
    hc::accelerator_view av = default_acc.get_default_view();
    hc::accelerator_view default_acc_view = default_acc.get_default_view();
    hc::accelerator_view* accl_view = NULL;

    status = hcrngLfsr113SetAcclView(creator, default_acc_view);
    EXPECT_EQ(status, HCRNG_SUCCESS);
    // Now Get the Accl_view
    status = hcrngLfsr113GetAcclView(creator, accl_view, stream);
    EXPECT_EQ(status, HCRNG_SUCCESS);

    EXPECT_TRUE(accl_view != NULL);
    if (default_acc_view == *accl_view) {
        EXPECT_EQ(status, HCRNG_SUCCESS);
    }
    // We must expect the accl_view obtained is what that's being set
    EXPECT_EQ(default_acc_view, *accl_view);
}

TEST(hcrng_SetGetAcclView, Return_Check_SetGetAcclView_Philox432 ) {
    hcrngPhilox432StreamCreator* creator = NULL;
    creator = (hcrngPhilox432StreamCreator*)malloc(sizeof(hcrngPhilox432StreamCreator));
    *(hcrngPhilox432StreamCreator*)creator = defaultStreamCreator_Philox432;
    hcrngStatus status;

    void **stream = NULL;
    hc::accelerator default_acc;
    hc::accelerator_view av = default_acc.get_default_view();
    hc::accelerator_view default_acc_view = default_acc.get_default_view();
    hc::accelerator_view* accl_view = NULL;

    status = hcrngPhilox432SetAcclView(creator, default_acc_view);
    EXPECT_EQ(status, HCRNG_SUCCESS);
    // Now Get the Accl_view
    status = hcrngPhilox432GetAcclView(creator, accl_view, stream);
    EXPECT_EQ(status, HCRNG_SUCCESS);

    EXPECT_TRUE(accl_view != NULL);
    if (default_acc_view == *accl_view) {
        EXPECT_EQ(status, HCRNG_SUCCESS);
    }
    // We must expect the accl_view obtained is what that's being set
    EXPECT_EQ(default_acc_view, *accl_view);
}
