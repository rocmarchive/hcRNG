## A. Introduction: ##

The hcRNG library is an implementation of uniform random number generators targetting the AMD heterogenous hardware via HCC compiler runtime. The computational resources of underlying AMD heterogenous compute gets exposed and exploited through the HCC C++ frontend. Refer [here](https://github.com/RadeonOpenCompute/hcc) for more details on HCC compiler.

The following list enumerates the current set of RNG generators that are supported so far.

1. MRG31k3p
2. MRG32k3a
3. LFSR113
4. Philox-4x32-10

To know more, go through the [Documentation](https://github.com/ROCmSoftwarePlatform/hcRNG/wiki)

## B. Key Features ##

* Support for 4 commonly used uniform random number generators.
* Single and Double precision.
* Multiple streams, created on the host and generates random numbers either on the host or on computing devices.

## C. Prerequisites ##

* Refer Prerequisites section [here](https://github.com/ROCmSoftwarePlatform/hcRNG/wiki/Prerequisites)

## D. Tested Environment so far 

* Refer Tested environments enumerated [here](https://github.com/ROCmSoftwarePlatform/hcRNG/wiki/Tested-Environments)

## E. Installation  

* Follow installation steps as described [here](https://github.com/ROCmSoftwarePlatform/hcRNG/wiki/Installation)

## F. Unit testing

* Follow testing procedures as explained [here](https://github.com/ROCmSoftwarePlatform/hcRNG/wiki/Unit-testing)

## G. API reference

* The Specification of API's supported along with description  can be found [here](http://hcrng-documentation.readthedocs.org/en/latest/API_reference.html)

## H. Example Code

* Refer Examples section [here](https://github.com/ROCmSoftwarePlatform/hcRNG/wiki/Examples)
