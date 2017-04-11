#include <cstdlib>
#include <cmath>
#include <limits>
#include "hcRNG.h"
#include <hc.hpp>
#include <hc_am.hpp>
using namespace hc;
#define BLOCK_SIZE 256

//HCRNGAPI hcrngStatus box_muller_transform_single(hc::accelerator_view &accl_view, float mu, float sigma, float *OutBuffer, size_t numberCount);
HCRNGAPI hcrngStatus box_muller_transform_double(hc::accelerator_view &accl_view, double mu, double sigma, double *OutBuffer, size_t numberCount);

//Device function
inline HCRNGAPI hcrngStatus box_muller_transform_single(float mu, float sigma, float* Randomnum, size_t numberCount) {
    const float two_pi = 2.0 * 3.14159265358979323846;
    long size = (numberCount/2 + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    hc::extent<1> grdExt(size);
    hc::tiled_extent<1> t_ext(grdExt, BLOCK_SIZE);
#if defined(__HCC_ACCELERATOR__) && (__HCC_ACCELERATOR__ != 0)      
      std::vector<hc::accelerator>acc = hc::accelerator::get_all();
      accelerator_view accl_view = (acc[1].get_default_view());
      hc::parallel_for_each(accl_view, t_ext, [ = ] (hc::tiled_index<1> tidx) __attribute__((hc, cpu)) {
      float z0, z1;
      int gid = tidx.global[0];
      if (gid < numberCount/2)
      {
        z0 = fast_math::sqrt(-2.0 * fast_math::log(Randomnum[gid * 2])) * fast_math::cos(two_pi * Randomnum[gid * 2 + 1]);
        z1 = fast_math::sqrt(-2.0 * fast_math::log(Randomnum[gid * 2])) * fast_math::sin(two_pi * Randomnum[gid * 2 + 1]);
        Randomnum[gid * 2] = z0 * sigma + mu;
        Randomnum[gid * 2 + 1] = z1 * sigma + mu;
      }
    });
#else
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].get_default_view());
    hc::parallel_for_each(accl_view, t_ext, [ = ] (hc::tiled_index<1> tidx) __attribute__((hc, cpu)) {
      float z0, z1;
      int gid = tidx.global[0];
      if (gid < numberCount/2)
      {
        z0 = fast_math::sqrt(-2.0 * fast_math::log(Randomnum[gid * 2])) * fast_math::cos(two_pi * Randomnum[gid * 2 + 1]);
        z1 = fast_math::sqrt(-2.0 * fast_math::log(Randomnum[gid * 2])) * fast_math::sin(two_pi * Randomnum[gid * 2 + 1]);
        Randomnum[gid * 2] = z0 * sigma + mu;
        Randomnum[gid * 2 + 1] = z1 * sigma + mu;
      }
    }).wait();
#endif
  return HCRNG_SUCCESS;
}

