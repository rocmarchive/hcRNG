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
        size_t streamBufferSize;
        size_t NbrStreams = 1;
        size_t streamCount = 10;
        size_t numberCount = 100;
        float *Random = (float*) malloc(sizeof(float) * numberCount);
        Concurrency::array_view<float> outBuffer(numberCount, Random);
        std::cout << "DeviceRandomU01Array with Philox432 :" << std::endl;
        hcrngPhilox432Stream *streams = hcrngPhilox432CreateStreams(NULL, streamCount, &streamBufferSize, NULL);
        Concurrency::array_view<hcrngPhilox432Stream> streams_buffer(streamCount, streams);
        status = hcrngPhilox432DeviceRandomU01Array_single(streamCount, streams_buffer, numberCount, outBuffer);
        if(status) std::cout << "TEST FAILED" << std::endl;
        for(int i =0; i < numberCount; i++)
         std::cout << "OUT["<<i<<"] = " << outBuffer[i] << std::endl;
        return 0;
}
