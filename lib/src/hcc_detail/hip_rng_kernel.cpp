#include "hcc_detail/hip_rng_kernel.h"
#include <hcRNG/mtgp32.h>
#include <hip/hip_hcc.h>

// DEVICE APIS 
// *****************************************************************************************************************
/*__host__ hiprngStatus_t hiprngMakeMTGP32Constants ( const mtgp32_params_fast_t params[], mtgp32_kernel_params_t* p )
    Set up constant parameters for the mtgp32 generator.
    Parameters

    params
        - Pointer to an array of type mtgp32_params_fast_t in host memory 
    p
        - pointer to a structure of type mtgp32_kernel_params_t in device memory.
*/


__host__ hiprngStatus_t hiprngMakeMTGP32Constants ( const mtgp32_params_fast_t params[], mtgp32_kernel_params_t* p ) {
  // TODO: Currently getting default accl_view from current device ID. Got to use the streamId set by user using hiprngSetStream
  int device_id;
  hipError_t err = hipGetDevice(&device_id); 
  if (err != hipSuccess) return HIPRNG_STATUS_ALLOCATION_FAILED;
  hc::accelerator current_accl; 
  err = hipHccGetAccelerator(device_id, &current_accl);
  if (err != hipSuccess) return HIPRNG_STATUS_ALLOCATION_FAILED;
  hc::accelerator_view current_default_accl_view = current_accl.get_default_view();

  // Invoke the init params kernel
  int ret = mtgp32_init_params_kernel(current_default_accl_view, params, p); 
  if (ret != 0) return HIPRNG_STATUS_INITIALIZATION_FAILED;
  return HIPRNG_STATUS_SUCCESS;
}


__host__ hiprngStatus_t hiprngMakeMTGP32KernelState(hiprngStateMtgp32_t *s,
                                            mtgp32_params_fast_t params[],
                                            mtgp32_kernel_params_t *k,
                                            int n,
                                            unsigned long long seed) {
  // TODO: Currently getting default accl_view from current device ID. Got to use the streamId set by user using hiprngSetStream
  int device_id;
  hipError_t err = hipGetDevice(&device_id); 
  if (err != hipSuccess) return HIPRNG_STATUS_ALLOCATION_FAILED;
  hc::accelerator current_accl; 
  err = hipHccGetAccelerator(device_id, &current_accl);
  if (err != hipSuccess) return HIPRNG_STATUS_ALLOCATION_FAILED;
  hc::accelerator_view current_default_accl_view = current_accl.get_default_view();

  // Invoke the init_seed kernel
  int ret = mtgp32_init_seed_kernel(current_default_accl_view, s, k, n, seed); 
  if (ret != 0) return HIPRNG_STATUS_INITIALIZATION_FAILED;
  return HIPRNG_STATUS_SUCCESS;
}







