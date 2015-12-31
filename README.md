# ** HCRNG ** #

##Introduction: ##

This repository hosts the HCC implementation of uniform random number generators. The following are the sub-routines that are implemented

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

  [Compiler-Debians](https://multicorewareinc.egnyte.com/dl/TD5IwsNEx3)

  Install the package hcc-0.8.1544-a9f4d2f-ddba18d-Linux.deb

  using the command,

    sudo dpkg -i <package_name>
      e.g. sudo dpkg -i  hcc-0.8.1544-a9f4d2f-ddba18d-Linux.deb

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

