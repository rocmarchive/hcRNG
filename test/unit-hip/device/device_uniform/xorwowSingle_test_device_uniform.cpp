#include "../../gtest/gtest.h"
// HIP runtime
#include <hip/hip_runtime.h>
#include <hiprng.h>
#include <hiprng_kernel.h>
// HIP and hcrng functions
#include <helper_functions.h>

/* this GPU kernel function calculates a random number and stores it in the parameter */
__global__ void random(hipLaunchParm lp, float* result, unsigned long long seed) {
  /* CUDA's random number library uses curandState_t to keep track of the seed value
     we will store a random state for every thread  */
  hiprngStateXORWOW_t state;

  /* we have to initialize the state */
  hiprng_init(seed, /* the seed controls the sequence of random values that are produced */
              0, /* the sequence number is only important with multiple cores */
              0, /* the offset is how much extra we advance in the sequence for each call, can be 0 */
              &state);

  /* curand works like rand - except that it takes a state as a parameter */
  *result = hiprng_uniform(&state);
}

TEST(hiprng_device_uniform, hip_device_xorwowSingle_uniform ) {

  /* allocate an int on the GPU */
  float* host_x, *gpu_x;
  float *hostData1, *devData1,*hostData2, *devData2, *hostData3, *devData3;
  float x, y;
  int ispassed = 0;

  hostData1 = (float *)calloc(1, sizeof(float));
  hipMalloc((void **)&devData1, sizeof(float));

  /* invoke the GPU to initialize all of the random states */
  hipLaunchKernel(random,
                dim3(1), dim3(1), 0, 0,
                devData1, 5000);

  /* copy the random number back */
  hipMemcpy(&x, devData1, sizeof(float), hipMemcpyDeviceToHost);

  hostData2 = (float *)calloc(1, sizeof(float));
  hipMalloc((void **)&devData2, sizeof(float));

  /* invoke the GPU to initialize all of the random states */
  hipLaunchKernel(random,
                dim3(1), dim3(1), 0, 0,
                devData2, 5000);

  /* copy the random number back */
  hipMemcpy(&y, devData2, sizeof(float), hipMemcpyDeviceToHost);

  // Compare outputs
  if (x == y)
    ispassed = 1;
  EXPECT_NEAR(ispassed , 1 , 0);

  hostData3 = (float *)calloc(1, sizeof(float));
  hipMalloc((void **)&devData3, sizeof(float));

  /* invoke the GPU to initialize all of the random states */
  hipLaunchKernel(random,
                dim3(1), dim3(1), 0, 0,
                devData3, 0);

  /* copy the random number back */
  hipMemcpy(&x, devData2, sizeof(float), hipMemcpyDeviceToHost);

  // Compare outputs
  if (x != y)
    ispassed = 1;
  EXPECT_NEAR(ispassed , 1 , 0);

  /* free the memory we allocated */
  hipFree(devData1);
  hipFree(devData2);
  hipFree(devData3);

  free(hostData1);
  free(hostData2);
  free(hostData3);
}
