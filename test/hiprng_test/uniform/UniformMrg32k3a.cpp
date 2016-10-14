#include <stdio.h>
#include <stdlib.h>
//#include <cuda.h>
//#include <curand.h>
#include <time.h>

// Utilities and system includes
#include <assert.h>
#include <helper_string.h>  // helper for shared functions common to CUDA Samples
// HIP runtime
#include <hip_runtime.h>
#include <hiprng.h>
// HIP and hcrng functions
#include <helper_functions.h>

int main(int argc, char *argv[]) {
  size_t n = 10;
  hiprngGenerator_t gen;
  float *hostDataUniform, *devDataUniform;

  // Allocate n floats on host
  hostDataUniform = (float *)calloc(n, sizeof(float));

  // Allocate n floats on device
  hipMalloc((void **)&devDataUniform, n * sizeof(float));


  //  Create pseudo-random number generator
  hiprngCreateGenerator(&gen, HIPRNG_PSEUDO_LFSR113);
  //  Set seed
//  std::cout << gen->initialState.g1[0]<<std::endl;
  hiprngSetPseudoRandomGeneratorSeed(gen,100);
// std::cout << gen->initialState.g1[0]<<std::endl;
  printf("Generating random numbers Uniform distribution (single precision) using MRG32K3A...");

  hiprngGenerateNormal(gen, devDataUniform, n, 0.0, 1.0);

  printf("\ndone.\n");

  hipMemcpy(hostDataUniform, devDataUniform, n * sizeof(float), hipMemcpyDeviceToHost);

  
  for(int i = 0; i < n ; i++)
    printf("%f,", hostDataUniform[i]);
  
  //Cleanup
  printf("\n");
  //curandDestroyGenerator(gen);

  hipFree(devDataUniform);

 
  free(hostDataUniform);

}
