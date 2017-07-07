#include "../gtest/gtest.h"
// HIP runtime
#include <hip/hip_runtime_api.h>
#include <hiprng.h>
// HIP and hcrng functions
#include <helper_functions.h>

TEST(hiprng_common, hip_xorwowGenerate ){

  size_t n = 20000;
  hiprngGenerator_t gen;
  bool ispassed = 1;
  unsigned int *hostData1, *devData1,*hostData2, *devData2, *hostData3, *devData3;

  // Allocate n unsigned ints on host
  hostData1 = (unsigned int *)calloc(n, sizeof(unsigned int));

  // Allocate n unsigned ints on device
  hipMalloc((void **)&devData1, n * sizeof(unsigned int));

  //  Create pseudo-random number generator
  hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_XORWOW);
  //  Set seed
  hiprngSetPseudoRandomGeneratorSeed(gen,0);
  // Generate random numbers
  hiprngStatus_t status = hiprngGenerate(gen, devData1, n);
 
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);

  hipMemcpy(hostData1, devData1, n * sizeof(unsigned int), hipMemcpyDeviceToHost);

  // Generate another set of random numbers with same seeed value

  // Allocate n unsigned ints on host
  hostData2 = (unsigned int *)calloc(n, sizeof(unsigned int));

  // Allocate n unsigned ints on device
  hipMalloc((void **)&devData2, n * sizeof(unsigned int));

  //  Create pseudo-random number generator
  hiprngCreateGenerator(&gen, HIPRNG_RNG_PSEUDO_XORWOW);
  //  Set seed
  hiprngSetPseudoRandomGeneratorSeed(gen,0);
  // Generate random numbers
  status = hiprngGenerate(gen, devData2, n);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);
  hipMemcpy(hostData2, devData2, n * sizeof(unsigned int), hipMemcpyDeviceToHost);
  //set different seed value
  hiprngSetPseudoRandomGeneratorSeed(gen,5000);

  // Allocate n unsigned ints on host
  hostData3 = (unsigned int *)calloc(n, sizeof(unsigned int));

  // Allocate n unsigned ints on device
  hipMalloc((void **)&devData3, n * sizeof(unsigned int));
  // Generate random numbers
  status = hiprngGenerate(gen, devData3, n);
  EXPECT_EQ(status, HIPRNG_STATUS_SUCCESS);
  hipMemcpy(hostData3, devData3, n * sizeof(unsigned int), hipMemcpyDeviceToHost);

  // Compare outputs
  for(int i =0; i < n; i++) {
std::cout << i << " " << hostData1[i] << std::endl;
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
