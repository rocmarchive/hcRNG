#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <hcRNG/mrg31k3p.h>
#include <hcRNG/hcRNG.h>
#include <amp.h>
using namespace Concurrency;

int main()
{
        hcrngMrg31k3pStream* stream = NULL;
        hcrngStatus status = HCRNG_SUCCESS;
        bool ispassed = 1;
        size_t streamBufferSize;
        size_t streamCount = 10;
        size_t numberCount = 100;
        int stream_length = -5;
        size_t streams_per_thread = 2;
        double *Random1 = (double*) malloc(sizeof(double) * numberCount);
        double *Random2 = (double*) malloc(sizeof(double) * numberCount);
        Concurrency::array_view<double> outBufferDevice(numberCount, Random1);
        Concurrency::array_view<double> outBufferHost(numberCount, Random2);
        hcrngMrg31k3pStream *streams = hcrngMrg31k3pCreateStreams(NULL, streamCount, &streamBufferSize, NULL);
        Concurrency::array_view<hcrngMrg31k3pStream> streams_buffer(streamCount, streams);
        status = hcrngMrg31k3pDeviceRandomU01Array_double(streamCount, streams_buffer, numberCount, outBufferDevice);
        if(status) std::cout << "TEST FAILED" << std::endl;
         for (size_t i = 0; i < numberCount; i++)
            outBufferHost[i] = hcrngMrg31k3pRandomU01(&streams[i % streamCount]);   
        for(int i =0; i < numberCount; i++) {
           if (outBufferDevice[i] != outBufferHost[i]){
                ispassed = 0;
                std::cout <<" RANDDEVICE[" << i<< "] " << outBufferDevice[i] << "and RANDHOST[" << i <<"] mismatches"<< outBufferHost[i] << std::endl;
                break;
            }
            else
                continue;
        }
        if(!ispassed) std::cout << "TEST FAILED" << std::endl;
        double *Random3 = (double*) malloc(sizeof(double) * numberCount);
        double *Random4 = (double*) malloc(sizeof(double) * numberCount);
        Concurrency::array_view<double> outBufferDevice_substream(numberCount, Random3);
        status = hcrngMrg31k3pDeviceRandomU01Array_double(streamCount, streams_buffer, numberCount, outBufferDevice_substream, stream_length, streams_per_thread);
        if(status) std::cout << "TEST FAILED" << std::endl;
       /* for( int i =0 ; i < numberCount; i++)
            std::cout <<" RANDDEVICE[" << i<< "] " << outBufferDevice_substream[i] <<std::endl;
       */ return 0;
}


