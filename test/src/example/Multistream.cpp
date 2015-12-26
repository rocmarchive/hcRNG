//Example on Multistream random number generation with Mrg31k3p generator 
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

//Multistream generation with host 
void multistream_fill_array(size_t spwi, size_t gsize, size_t quota, int substream_length, hcrngMrg31k3pStream* streams, fp_type* out_)
{
  for (size_t i = 0; i < quota; i++) {
      for (size_t gid = 0; gid < gsize; gid++) {
      	//Create streams 
          hcrngMrg31k3pStream* s = &streams[spwi * gid];
          fp_type* out = &out_[spwi * (i * gsize + gid)];
          //Do nothing when subtsream_length is equal to 0
          if ((i > 0) && (substream_length > 0) && (i % substream_length == 0))
          //Forward to next substream when substream_length is greater than 0
              hcrngMrg31k3pForwardToNextSubstreams(spwi, s);
          else if ((i > 0) && (substream_length < 0) && (i % (-substream_length) == 0))
          //Rewind substreams when substream_length is smaller than 0
              hcrngMrg31k3pRewindSubstreams(spwi, s);
          //Generate Random Numbers
          for (size_t sid = 0; sid < spwi; sid++) {
              out[sid] = hcrngMrg31k3pRandomU01(&s[sid]);
          }
      }
  }
}

int main()
{
        hcrngStatus status = HCRNG_SUCCESS;
        bool ispassed = 1;
        size_t streamBufferSize;
        //Number of streams 
        size_t streamCount = 10;
        //Number of Random numbers to be generated (numberCount should be a multiple of streamCount)
        size_t numberCount = 100;
        //Substream length
        //Substream_length       = 0   // do not use substreams
        //Substream_length       = > 0   // go to next substreams after Substream_length values
        //Substream_length       = < 0  // restart substream after Substream_length values
        int stream_length = 5; 
        size_t streams_per_thread = 2;
        
        //Allocate Host pointers 
        fp_type *Random1 = (fp_type*) malloc(sizeof(fp_type) * numberCount);
        fp_type *Random2 = (fp_type*) malloc(sizeof(fp_type) * numberCount);
        //Allocate buffer for Device output
        hc::array_view<fp_type> outBufferDevice_substream(numberCount, Random1);
        hcrngMrg31k3pStream *streams = hcrngMrg31k3pCreateStreams(NULL, streamCount, &streamBufferSize, NULL);
        hc::array_view<hcrngMrg31k3pStream> streams_buffer(streamCount, streams);        

        //Invoke Random number generator function in Device
#ifdef HCRNG_SINGLE_PRECISION        	
        status = hcrngMrg31k3pDeviceRandomU01Array_single(streamCount, streams_buffer, numberCount, outBufferDevice_substream, stream_length, streams_per_thread);
#else
      	status = hcrngMrg31k3pDeviceRandomU01Array_double(streamCount, streams_buffer, numberCount, outBufferDevice_substream, stream_length, streams_per_thread);
#endif       	
        //Status check
        if(status) std::cout << "TEST FAILED" << std::endl;
        
        //Invoke random number generator from Host
        multistream_fill_array(streams_per_thread, streamCount/streams_per_thread, numberCount/streamCount, stream_length, streams, Random2);
        
        //Compare Host and device outputs
        for(int i =0; i < numberCount; i++) {
           if (outBufferDevice_substream[i] != Random2[i]) {
                ispassed = 0;
                std::cout <<" RANDDEVICE_SUBSTREAM[" << i<< "] " << outBufferDevice_substream[i] << "and RANDHOST_SUBSTREAM[" << i <<"] mismatches"<< Random2[i] << std::endl;
                break;
            }
            else
                continue;
        }
        if(!ispassed) std::cout << "TEST FAILED" << std::endl;
        return 0;     
}
