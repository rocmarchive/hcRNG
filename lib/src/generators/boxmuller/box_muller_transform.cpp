#include "hcRNG/box_muller_transform.h"
#include "hcRNG/hcRNG.h"
#include <hc.hpp>
using namespace hc;
#define BLOCK_SIZE 256

hcrngStatus box_muller_transform_single(hc::accelerator_view &accl_view, float mu, float sigma, float* Randomnum, size_t numberCount) {
    const float two_pi = 2.0 * 3.14159265358979323846;
    long size = (numberCount/2 + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    hc::extent<1> grdExt(size);
    hc::tiled_extent<1> t_ext(grdExt, BLOCK_SIZE);
    hc::parallel_for_each(accl_view, t_ext, [ = ] (hc::tiled_index<1> tidx) [[hc]] {
      float z0, z1;
      int gid = tidx.global[0];
      if (gid < numberCount/2)
      {
        z0 = fast_math::sqrt(-2.0f * fast_math::log(Randomnum[gid * 2])) *
            fast_math::cos(two_pi * Randomnum[gid * 2 + 1]);
        z1 = fast_math::sqrt(-2.0f * fast_math::log(Randomnum[gid * 2])) *
            fast_math::sin(two_pi * Randomnum[gid * 2 + 1]);
        Randomnum[gid * 2] = z0 * sigma + mu;
        Randomnum[gid * 2 + 1] = z1 * sigma + mu;
      }
    }).wait();
  return HCRNG_SUCCESS;
}

hcrngStatus box_muller_transform_double(hc::accelerator_view &accl_view, double mu, double sigma, double* Randomnum, size_t numberCount) {
    const double two_pi = 2.0 * 3.14159265358979323846;
    long size = (numberCount/2 + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    hc::extent<1> grdExt(size);
    hc::tiled_extent<1> t_ext(grdExt, BLOCK_SIZE);
    hc::parallel_for_each(accl_view, t_ext, [=] (hc::tiled_index<1> tidx) [[hc]] {
      double z0, z1;
      int gid = tidx.global[0];
      if (gid < numberCount/2)
      {
        z0 = precise_math::sqrt(-2.0 * precise_math::log(Randomnum[gid * 2])) *
            precise_math::cos(two_pi * Randomnum[gid * 2 + 1]);
        z1 = precise_math::sqrt(-2.0 * precise_math::log(Randomnum[gid * 2])) *
            precise_math::sin(two_pi * Randomnum[gid * 2 + 1]);
        Randomnum[gid * 2] = z0 * sigma + mu;
        Randomnum[gid * 2 + 1] = z1 * sigma + mu;
      }
    }).wait();
  return HCRNG_SUCCESS;
}
