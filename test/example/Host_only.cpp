//This example shows how to invoke Random generators from Host
//Example with Mrg31k3p Random number generator
#include <stdio.h>
#include <iostream>
#include <hcRNG/mrg31k3p.h>

int main() {
	
//Create Streams
    hcrngMrg31k3pStream* streams = hcrngMrg31k3pCreateStreams(NULL, 2, NULL, NULL);
    for (int i = 0; i < 100; i++) {
//Calling RandomNormal function from host that generates random numbers on "streams"
        double u = hcrngMrg31k3pRandomN(&streams[i % 2], &streams[(i + 1) % 2], 0.0, 1.0);
    }
    return 0;
}
