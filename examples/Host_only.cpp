/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// This example shows how to invoke Random generators from Host
// Example with Mrg31k3p Random number generator
#include <hcRNG/mrg31k3p.h>
#include <iostream>
#include <stdio.h>

int main() {
  // Create Streams
  hcrngMrg31k3pStream* streams =
      hcrngMrg31k3pCreateStreams(NULL, 2, NULL, NULL);
  hcrngMrg31k3pStream* single = hcrngMrg31k3pCreateStreams(NULL, 1, NULL, NULL);

  // Initialize the count
  int count = 0;
  for (int i = 0; i < 100; i++) {
    // Calling RandomU01 function from host that generates random numbers on
    // "streams"
    double u = hcrngMrg31k3pRandomU01(&streams[i % 2]);

    // Calling RandomInteger funcion from host that generated random numbers on
    // a single stream
    int x = hcrngMrg31k3pRandomInteger(single, 1, 6);
    if (x * u < 2) count++;
  }
  std::cout << "Average of indicators = " << static_cast<double>(count) / 100.0 << std::endl;
  return 0;
}
