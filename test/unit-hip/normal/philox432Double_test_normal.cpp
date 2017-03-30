#include "../gtest/gtest.h"
// HIP runtime
#include <hip/hip_runtime.h>
#include <hiprng.h>
// HIP and hcrng functions
#include <helper_functions.h>

TEST(hiprng_normal, hip_philox432Double_normal ) {

  size_t n = 100;
  hiprngGenerator_t gen;
  bool ispassed = 1;
  double *hostData1, *devData1,*hostData2, *devData2, *hostData3, *devData3;

  // Allocate n doubles on host
  hostData1 = (double *)calloc(n, sizeof(double));

  // Allocate n doubles on device
  hipMalloc((void **)&devData1, n * sizeof(double));

  //  Create pseudo-random number generator
  hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_PHILOX432);
  //  Set seed
  hiprngSetPseudoRandomGeneratorSeed(gen,0);
  // Generate random numbers
  hiprngStatus_t status = hiprngGenerateNormalDouble(gen, devData1, n, 0.0, 1.0);
 
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hipMemcpy(hostData1, devData1, n * sizeof(double), hipMemcpyDeviceToHost);

  // Generate another set of random numbers with same seeed value

  // Allocate n doubles on host
  hostData2 = (double *)calloc(n, sizeof(double));

  // Allocate n doubles on device
  hipMalloc((void **)&devData2, n * sizeof(double));

  //  Create pseudo-random number generator
  hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_PHILOX432);
  //  Set seed
  hiprngSetPseudoRandomGeneratorSeed(gen,0);
  // Generate random numbers
  status = hiprngGenerateNormalDouble(gen, devData2, n, 0.0, 1.0);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hipMemcpy(hostData2, devData2, n * sizeof(double), hipMemcpyDeviceToHost);
  //set different seed value
  hiprngSetPseudoRandomGeneratorSeed(gen,5000);

  // Allocate n doubles on host
  hostData3 = (double *)calloc(n, sizeof(double));

  // Allocate n doubles on device
  hipMalloc((void **)&devData3, n * sizeof(double));
  // Generate random numbers
  status = hiprngGenerateNormalDouble(gen, devData3, n, 0.0, 1.0);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hipMemcpy(hostData3, devData3, n * sizeof(double), hipMemcpyDeviceToHost);

  // Compare outputs
  for(int i =0; i < n; i++) {
    if (hostData1[i] != hostData2[i]) {
      ispassed = 0;
      break;
    }
   else
    continue;
   }
  EXPECT_NEAR(ispassed , 1 , 0);

  // Compare outputs
  for(int i =0; i < n; i++) {
    if (hostData1[i] == hostData3[i]) {
      ispassed = 0;
      break;
    }
   else
    continue;
   }
  EXPECT_NEAR(ispassed , 1 , 0);

  //Cleanup
  hiprngDestroyGenerator(gen);
  hipFree(devData1);
  hipFree(devData2);
  hipFree(devData3);
 
  free(hostData1);
  free(hostData2);
  free(hostData3);
}
