/*  @file box_muller_transform.h
*  @brief Specific interface for the Box Muller Transform generator
*/

#pragma once
#ifndef BOX_MULLER_TRANSFORM_H
#define BOX_MULLER_TRANSFORM_H
#include "hcRNG.h"
#include <stdio.h>

namespace hc {
  class accelerator;
  class accelerator_view;
};

HCRNGAPI hcrngStatus box_muller_transform_single(hc::accelerator_view &accl_view, float mu, float sigma, float *OutBuffer, size_t numberCount);

HCRNGAPI hcrngStatus box_muller_transform_double(hc::accelerator_view &accl_view, double mu, double sigma, double *OutBuffer, size_t numberCount);

#endif // BOX_MULLER_TRANSFORM_H
