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
using namespace hc;

#define HCRNG_SINGLE_PRECISION
#ifdef HCRNG_SINGLE_PRECISION
typedef float fp_type;
#else
typedef double fp_type;
#endif


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

        //Allocate memory for host pointers
        fp_type *Random1 = (fp_type*) malloc(sizeof(fp_type) * numberCount);
        fp_type *Random2 = (fp_type*) malloc(sizeof(fp_type) * numberCount);
        hc::array_view<fp_type> outBufferDevice(numberCount, Random1);
        hc::array_view<fp_type> outBufferHost(numberCount, Random2);
  
        //Create streams
        hcrngMrg31k3pStream *streams = hcrngMrg31k3pCreateStreams(NULL, streamCount, &streamBufferSize, NULL);
        hc::array_view<hcrngMrg31k3pStream> streams_buffer(streamCount, streams);

        //Invoke random number generators in device (here strean_length and streams_per_thread arguments are default) 
#ifdef HCRNG_SINGLE_PRECISION
        status = hcrngMrg31k3pDeviceRandomU01Array_single(streamCount, streams_buffer, numberCount, outBufferDevice);
#else
        status = hcrngMrg31k3pDeviceRandomU01Array_double(streamCount, streams_buffer, numberCount, outBufferDevice);
#endif
        if(status) std::cout << "TEST FAILED" << std::endl;

        //Invoke random number generators in host
        for (size_t i = 0; i < numberCount; i++)
            outBufferHost[i] = hcrngMrg31k3pRandomU01(&streams[i % streamCount]);   

        // Compare host and device outputs
        for(int i =0; i < numberCount; i++) {
           if (outBufferDevice[i] != outBufferHost[i]) {
                ispassed = 0;
                std::cout <<" RANDDEVICE[" << i<< "] " << outBufferDevice[i] << "and RANDHOST[" << i <<"] mismatches"<< outBufferHost[i] << std::endl;
                break;
            }
            else
                continue;
        }
        if(!ispassed) std::cout << "TEST FAILED" << std::endl;
        return 0;
}
