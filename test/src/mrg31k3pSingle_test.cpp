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
        size_t streamBufferSize;
        size_t NbrStreams = 1;
        size_t streamCount = 10;
        size_t numberCount = 100;
        float *Random = (float*) malloc(sizeof(float) * numberCount);
        Concurrency::array_view<float> outBuffer(numberCount, Random);
        std::cout << "DeviceRandomU01Array with Mrg31k3p :" << std::endl;
        hcrngMrg31k3pStream *streams = hcrngMrg31k3pCreateStreams(NULL, streamCount, &streamBufferSize, NULL);
        Concurrency::array_view<hcrngMrg31k3pStream> streams_buffer(streamCount, streams);
        status = hcrngMrg31k3pDeviceRandomU01Array_single(streamCount, streams_buffer, numberCount, outBuffer);
        if(status) std::cout << "TEST FAILED" << std::endl;
        for(int i =0; i < numberCount; i++)
         std::cout << "OUT["<<i<<"] = " << outBuffer[i] << std::endl;
        return 0;
}


