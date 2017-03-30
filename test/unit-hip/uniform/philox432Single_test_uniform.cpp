#include "../gtest/gtest.h"
// HIP runtime
#include <hip/hip_runtime.h>
#include <hiprng.h>
// HIP and hcrng functions
#include <helper_functions.h>

TEST(hiprng_uniform, hip_philox432Single_uniform ) {

  size_t n = 100;
  hiprngGenerator_t gen;
  bool ispassed = 1;
  float *hostData1, *devData1,*hostData2, *devData2, *hostData3, *devData3;

  // Allocate n floats on host
  hostData1 = (float *)calloc(n, sizeof(float));

  // Allocate n floats on device
  hipMalloc((void **)&devData1, n * sizeof(float));

  //  Create pseudo-random number generator
  hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_PHILOX432);
  //  Set seed
  hiprngSetPseudoRandomGeneratorSeed(gen,0);
  // Generate random numbers
  hiprngStatus_t status = hiprngGenerateUniform(gen, devData1, n);
 
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hipMemcpy(hostData1, devData1, n * sizeof(float), hipMemcpyDeviceToHost);

  // Generate another set of random numbers with same seeed value

  // Allocate n floats on host
  hostData2 = (float *)calloc(n, sizeof(float));

  // Allocate n floats on device
  hipMalloc((void **)&devData2, n * sizeof(float));

  //  Create pseudo-random number generator
  hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_PHILOX432);
  //  Set seed
  hiprngSetPseudoRandomGeneratorSeed(gen,0);
  // Generate random numbers
  status = hiprngGenerateUniform(gen, devData2, n);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hipMemcpy(hostData2, devData2, n * sizeof(float), hipMemcpyDeviceToHost);
  //set different seed value
  hiprngSetPseudoRandomGeneratorSeed(gen,5000);

  // Allocate n floats on host
  hostData3 = (float *)calloc(n, sizeof(float));

  // Allocate n floats on device
  hipMalloc((void **)&devData3, n * sizeof(float));
  // Generate random numbers
  status = hiprngGenerateUniform(gen, devData3, n);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hipMemcpy(hostData3, devData3, n * sizeof(float), hipMemcpyDeviceToHost);

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
