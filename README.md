# ** HCRNG ** #

##Introduction: ##

This repository hosts the C++ AMP implementation of uniform random number generators. The following are the sub-routines that are implemented

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

### A. C++ AMP Compiler Installation: 

Use either (i) or (ii) to install the compiler.

**(i) Install through debian package:**

  Download the debian package from the link given below,
  
  [Compiler-Debians](https://multicorewareinc.egnyte.com/fl/LYdifMPiOt)
  
  Install those packages in the following order,
  
    1. libcxxamp-0.3.0-milestone4-1506-ga1e5f-Linux
    2. clamp-0.3.0-milestone4-1506-ga1e5f-dirty-Linux
  
  using the command,
  
    sudo dpkg -i <package_name>
      e.g. sudo dpkg -i libcxxamp-0.3.0-milestone4-1506-ga1e5f-Linux.deb
      
  Note: 
      Ignore clamp-bolt, Bolt is not required for hcblas.
    
**(ii) Build from source **

  To build the compiler from source follow the steps given below,
 
  Make sure the parent directory chosen is say ~/ or any other folder of your choice. Lets take ~/ as an example

  (a) Prepare a directory for work space

       * mkdir ~/mcw_cppamp

       * cd ~/mcw_cppamp 
   
       * git clone https://bitbucket.org/multicoreware/cppamp-driver-ng.git src

       * cd ~/mcw_cppamp/src/

       * git checkout origin/torch-specific

  (b) Create a build directory and configure using CMake.

       * mkdir ~/mcw_cppamp/build

       * cd ~/mcw_cppamp/build

       * export CLAMP_NOTILECHECK=ON
       
       * cmake ../src -DCMAKE_BUILD_TYPE=Release -DCXXAMP_ENABLE_BOLT=ON -DOPENCL_HEADER_DIR=<path to SDK's OpenCL headers> -DOPENCL_LIBRARY_DIR=<path to SDK's OpenCL library>, 

       * for example cmake ../src -DCMAKE_BUILD_TYPE=Release -DCXXAMP_ENABLE_BOLT=ON  -DOPENCL_HEADER_DIR=/opt/AMDAPPSDK-2.9-1/include/CL/ -DOPENCL_LIBRARY_DIR=/opt/AMDAPPSDK-2.9-1/lib/x86_64/

  (c) Build AMP

       * cd ~/mcw_cppamp/build

       * make [-j#] world && make          (# is the number of parallel builds. Generally it is # of CPU cores)

       * For example: make -j8 world && make

With this the C++ AMP Compiler installation is complete.
