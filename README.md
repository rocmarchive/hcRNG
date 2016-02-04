# ** HCRNG ** #

##Introduction: ##

The hcRNG library is an implementation of uniform random number generators targetting the AMD heterogenous hardware via HCC compiler runtime. The computational resources of underlying AMD heterogenous compute gets exposed and exploited through the HCC C++ frontend. Refer [here](https://bitbucket.org/multicoreware/hcc/wiki/Home) for more details on HCC compiler.

The following list enumerates the current set of RNG generators that are supported so far.

1. MRG31k3p
2. MRG32k3a
3. LFSR113
4. Philox-4x32-10

##Repository Structure: ##

##Prerequisites: ##
* **dGPU**:  AMD firepro S9150
* **OS** : Ubuntu 14.04 LTS
* **Ubuntu Pack**: libc6-dev-i386
* **AMD APP SDK** : Ver 2.9.1 launched on 18/8/2014 from [here](http://developer.amd.com/tools-and-sdks/opencl-zone/amd-accelerated-parallel-processing-app-sdk/)
* **AMD Driver installer**: amd-driver-installer-14.301.1001-x86.x86_64


## Installation Steps:    

### A. HCC Compiler Installation: 


**Install HCC compiler debian package:**

  Download the debian package from the link given below,

  [Compiler-Debians](https://bitbucket.org/multicoreware/hcc/downloads/hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb)

  Install the package hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb

  using the command,

    sudo dpkg -i <package_name>
      e.g. sudo dpkg -i  hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb

  Note:
      Ignore clamp-bolt, Bolt is not required for hcRNG.


### B. HCRNG Installation 

Clone MCW HCRNG source codes

       * cd ~/

       * git clone https://bitbucket.org/multicoreware/hcrng.git

       * cd ~/hcrng

       * ./install.sh test=ON/OFF 
       Where
       test=OFF    - Build library and tests
       test=ON     - Build library, tests and run test.sh (do functionality check and gtest)

### C. Unit testing

### Testing:

     * cd ~/hcrng/test/

     * ./test.sh

### Manual testing: 

(i)   Functionality check

     * cd ~/hcrng/build/test/src/bin/
       Run all executables (stream and substream device kernel outputs are compared with host outputs).

(ii)  Google testing (GTEST)

     * cd ~/hcrng/build/test/unit/bin/
       All functions are tested against google test.

## D. Example Code

Random number generator Mrg31k3p example:

file: Randomarray.cpp

```
#!c++

//This example is a simple random array generation and it compares host output with device output
//Random number generator Mrg31k3p
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <hcRNG/mrg31k3p.h>
#include <hcRNG/hcRNG.h>
#include <hc.hpp>
#include <hc_am.hpp>
using namespace hc;

int main()
{
      hcrngStatus status = HCRNG_SUCCESS;
      bool ispassed = 1;
      size_t streamBufferSize;
      // Number oi streams
      size_t streamCount = 10;
      //Number of random numbers to be generated
      //numberCount must be a multiple of streamCount
      size_t numberCount = 100; 
      //Enumerate the list of accelerators
      std::vector<hc::accelerator>acc = hc::accelerator::get_all();
      accelerator_view accl_view = (acc[1].create_view());
      //Allocate memory for host pointers
      float *Random1 = (float*) malloc(sizeof(float) * numberCount);
      float *Random2 = (float*) malloc(sizeof(float) * numberCount);
      float *outBufferDevice = hc::am_alloc(sizeof(float) * numberCount, acc[1], 0);

      //Create streams
      hcrngMrg31k3pStream *streams = hcrngMrg31k3pCreateStreams(NULL, streamCount, &streamBufferSize, NULL);
      hcrngMrg31k3pStream *streams_buffer = hc::am_alloc(sizeof(hcrngMrg31k3pStream) * streamCount, acc[1], 0);
      hc::am_copy(streams_buffer, streams, streamCount* sizeof(hcrngMrg31k3pStream));

      //Invoke random number generators in device (here strean_length and streams_per_thread arguments are default) 
      status = hcrngMrg31k3pDeviceRandomU01Array_single(accl_view, streamCount, streams_buffer, numberCount, outBufferDevice);
 
      if(status) std::cout << "TEST FAILED" << std::endl;
      hc::am_copy(Random1, outBufferDevice, numberCount * sizeof(float));

      //Invoke random number generators in host
      for (size_t i = 0; i < numberCount; i++)
          Random2[i] = hcrngMrg31k3pRandomU01(&streams[i % streamCount]);   
      // Compare host and device outputs
      for(int i =0; i < numberCount; i++) {
          if (Random1[i] != Random2[i]) {
              ispassed = 0;
              std::cout <<" RANDDEVICE[" << i<< "] " << Random1[i] << "and RANDHOST[" << i <<"] mismatches"<< Random2[i] << std::endl;
              break;
          }
          else
              continue;
      }
      if(!ispassed) std::cout << "TEST FAILED" << std::endl;
 
      //Free host resources
      free(Random1);
      free(Random2);
      //Release device resources
      hc::am_free(outBufferDevice);
      hc::am_free(streams_buffer);
      return 0;
}  

```
* Compiling the example code:

          /opt/hcc/bin/clang++ `/opt/hcc/bin/hcc-config --cxxflags --ldflags` -lhc_am -lhcrng Randomarray.cpp

