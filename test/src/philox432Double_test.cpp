#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <hcRNG/philox432.h>
#include <hcRNG/hcRNG.h>
#include <amp.h>
using namespace Concurrency;

int main()
{
        hcrngPhilox432Stream* stream = NULL;
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
        hcrngPhilox432Stream *streams = hcrngPhilox432CreateStreams(NULL, streamCount, &streamBufferSize, NULL);
        Concurrency::array_view<hcrngPhilox432Stream> streams_buffer(streamCount, streams);
        status = hcrngPhilox432DeviceRandomU01Array_double(streamCount, streams_buffer, numberCount, outBufferDevice);
        if(status) std::cout << "TEST FAILED" << std::endl;
        for (size_t i = 0; i < numberCount; i++)
            outBufferHost[i] = hcrngPhilox432RandomU01(&streams[i % streamCount]);   
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
        double *Random3 = (double*) malloc(sizeof(double) * numberCount);
        double *Random4 = (double*) malloc(sizeof(double) * numberCount);
        Concurrency::array_view<double> outBufferDevice_substream(numberCount, Random3);
        status = hcrngPhilox432DeviceRandomU01Array_double(streamCount, streams_buffer, numberCount, outBufferDevice_substream, stream_length, streams_per_thread);
        if(status) std::cout << "TEST FAILED" << std::endl;
/*        for( int i =0 ; i < numberCount; i++)
            std::cout <<" RANDDEVICE[" << i<< "] " << outBufferDevice_substream[i] <<std::endl;
*/        return 0;
}


