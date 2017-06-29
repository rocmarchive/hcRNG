#include "../gtest/gtest.h"
// HIP runtime
#include <hip/hip_runtime_api.h>
#include <hiprng.h>
// HIP and hcrng functions
#include <helper_functions.h>

TEST(hiprng_common, hip_SetStream_Mrg31k3p) {
  
  hiprngGenerator_t gen;
  hiprngStatus_t status;

  //  Create pseudo-random number generator
  status = hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_MRG31K3P);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);
  
  status =  hiprngSetStream(&gen, hipStreamDefault); 
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);
  
  hiprngDestroyGenerator(gen);
}

TEST(hiprng_common, hip_SetStream_Mrg32k3a) {

  hiprngGenerator_t gen;
  hiprngStatus_t status;

  //  Create pseudo-random number generator
  status = hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_MRG32K3A);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  status =  hiprngSetStream(&gen, hipStreamDefault);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hiprngDestroyGenerator(gen);
}

TEST(hiprng_common, hip_SetStream_Lfsr113) {

  hiprngGenerator_t gen;
  hiprngStatus_t status;

  //  Create pseudo-random number generator
  status = hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_LFSR113);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  status =  hiprngSetStream(&gen, hipStreamDefault);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hiprngDestroyGenerator(gen);
}

TEST(hiprng_common, hip_SetStream_Philox432) {

  hiprngGenerator_t gen;
  hiprngStatus_t status;

  //  Create pseudo-random number generator
  status = hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_PHILOX432);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  status =  hiprngSetStream(&gen, hipStreamDefault);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hiprngDestroyGenerator(gen);
}
