#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <hcRNG/lfsr113.h>
#include <hcRNG/hcRNG.h>
#include <amp.h>
using namespace Concurrency;

int main()
{
        hcrngLfsr113Stream* stream = NULL;
        hcrngStatus status = HCRNG_SUCCESS;
        size_t streamBufferSize;
        size_t NbrStreams = 1;
        size_t streamCount = 100;
        size_t numberCount = 10000000;
        float *Random = (float*) malloc(sizeof(float) * numberCount);
        Concurrency::array_view<float> outBuffer(numberCount, Random);
        std::cout << "DeviceRandomU01Array with Lfsr113 :" << std::endl;
        hcrngLfsr113Stream *streams = hcrngLfsr113CreateStreams(NULL, streamCount, &streamBufferSize, NULL);
        Concurrency::array_view<hcrngLfsr113Stream> streams_buffer(streamCount, streams);
        status = hcrngLfsr113DeviceRandomU01Array_single(streamCount, streams_buffer, numberCount, outBuffer);
        if(status) std::cout << "TEST FAILED" << std::endl;
        for(int i =0; i < numberCount; i++)
         std::cout << "OUT["<<i<<"] = " << outBuffer[i] << std::endl;
        return 0;
}


