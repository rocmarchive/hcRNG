#include <hcRNG/hcRNG.h>
#include <hcRNG/mrg31k3p.h>
#include <hcRNG/mrg32k3a.h>
#include <hcRNG/lfsr113.h>
#include <hcRNG/philox432.h>
#include "gtest/gtest.h"

  
TEST(hcfft_Destroy_Stream_Creator, func_correct_Destroy_Stream_Creator_Mrg31k3p ) {
    hcrngMrg31k3pStreamCreator* creator1 = NULL;
    hcrngStatus status;
    status = hcrngMrg31k3pDestroyStreamCreator(creator1);
    EXPECT_EQ(status, HCRNG_SUCCESS);
    hcrngMrg31k3pStreamCreator* creator2 = hcrngMrg31k3pCopyStreamCreator(NULL, NULL);
    status = hcrngMrg31k3pDestroyStreamCreator(creator2);
    EXPECT_EQ(status, HCRNG_SUCCESS);
}

TEST(hcfft_Destroy_Stream_Creator, func_correct_Destroy_Stream_Creator_Mrg32k3a ) {
    hcrngMrg32k3aStreamCreator* creator1 = NULL;
    hcrngStatus status;
    status = hcrngMrg32k3aDestroyStreamCreator(creator1);
    EXPECT_EQ(status, HCRNG_SUCCESS);
    hcrngMrg32k3aStreamCreator* creator2 = hcrngMrg32k3aCopyStreamCreator(NULL, NULL);
    status = hcrngMrg32k3aDestroyStreamCreator(creator2);
    EXPECT_EQ(status, HCRNG_SUCCESS);
}

TEST(hcfft_Destroy_Stream_Creator, func_correct_Destroy_Stream_Creator_Lfsr113 ) {
    hcrngLfsr113StreamCreator* creator1 = NULL;
    hcrngStatus status;
    status = hcrngLfsr113DestroyStreamCreator(creator1);
    EXPECT_EQ(status, HCRNG_SUCCESS);
    hcrngLfsr113StreamCreator* creator2 = hcrngLfsr113CopyStreamCreator(NULL, NULL);
    status = hcrngLfsr113DestroyStreamCreator(creator2);
    EXPECT_EQ(status, HCRNG_SUCCESS);
}

TEST(hcfft_Destroy_Stream_Creator, func_correct_Destroy_Stream_Creator_Philox432 ) {
    hcrngPhilox432StreamCreator* creator1 = NULL;
    hcrngStatus status;
    status = hcrngPhilox432DestroyStreamCreator(creator1);
    EXPECT_EQ(status, HCRNG_SUCCESS);
    hcrngPhilox432StreamCreator* creator2 = hcrngPhilox432CopyStreamCreator(NULL, NULL);
    status = hcrngPhilox432DestroyStreamCreator(creator2);
    EXPECT_EQ(status, HCRNG_SUCCESS);
}
