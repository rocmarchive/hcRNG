===================
hcRNG documentation
===================

***************
1. Introduction
***************
-------------------------------------------------------------------------------------------------------------------------------------------

The hcRNG library is an implementation of uniform random number generators targetting the AMD heterogenous hardware via HCC compiler runtime. The computational resources of underlying AMD heterogenous compute gets exposed and exploited through the HCC C++ frontend. Refer `here <https://bitbucket.org/multicoreware/hcc/wiki/Home>`_ for more details on HCC compiler.

The following list enumerates the current set of RNG generators that are supported so far.

 1. MRG31k3p `[4] <bibliography.html>`_
 2. MRG32k3a `[7] <bibliography.html>`_
 3. LFSR113  `[8] <bibliography.html>`_
 4. Philox-4x32-10 `[11] <bibliography.html>`_

Library provides multiple streams that are created on the host computer and used to generate random numbers either on the host or on computing devices by work items. Such multiple streams are essential for parallel simulation `[6] <bibliography.html>`_ and are often useful as well for simulation on a single processing element (or within a single work item), for example when comparing similar systems via simulation with common random numbers `[1] <bibliography.html>`_, `[9] <bibliography.html>`_, `[10] <bibliography.html>`_, `[5] <bibliography.html>`_ . Streams can also be divided into segments of equal length called substreams, as in `[2] <bibliography.html>`_, `[5] <bibliography.html>`_, `[10] <bibliography.html>`_ .

*****************
2. Prerequisites
*****************
-------------------------------------------------------------------------------------------------------------------------------------------

This section lists the known set of hardware and software requirements to build this library

2.1. Hardware
^^^^^^^^^^^^^

* CPU: mainstream brand, Better if with >=4 Cores Intel Haswell based CPU
* System Memory >= 4GB (Better if >10GB for NN application over multiple GPUs)
* Hard Drive > 200GB (Better if SSD or NVMe driver  for NN application over multiple GPUs)
* Minimum GPU Memory (Global) > 2GB

2.2. GPU cards supported
^^^^^^^^^^^^^^^^^^^^^^^^

* dGPU: AMD R9 Fury X, R9 Fury, R9 Nano
* APU: AMD Kaveri or Carrizo

2.3 AMD Driver and Runtime
^^^^^^^^^^^^^^^^^^^^^^^^^^

* Radeon Open Compute Kernel (ROCK) driver : https://github.com/RadeonOpenCompute/ROCK-Kernel-Driver
* HSA runtime API and runtime for Boltzmann:  https://github.com/RadeonOpenCompute/ROCR-Runtime

2.4. System software
^^^^^^^^^^^^^^^^^^^^^

* Ubuntu 14.04 trusty
* GCC 4.6 and later
* CPP 4.6 and later (come with GCC package)
* python 2.7 and later
* HCC 0.9 from `here <https://bitbucket.org/multicoreware/hcc/downloads/hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb>`_


2.5. Tools and Misc
^^^^^^^^^^^^^^^^^^^

* git 1.9 and later
* cmake 2.6 and later (2.6 and 2.8 are tested)
* firewall off
* root privilege or user account in sudo group

2.6. Ubuntu Packages
^^^^^^^^^^^^^^^^^^^^

* libc6-dev-i386
* liblapack-dev
* graphicsmagick

**********************
3. Tested Environments
**********************
-------------------------------------------------------------------------------------------------------------------------------------------

This sections enumerates the list of tested combinations of Hardware and system softwares.

3.1. Driver versions 
^^^^^^^^^^^^^^^^^^^^

* Boltzmann Early Release Driver + dGPU

      * Radeon Open Compute Kernel (ROCK) driver : https://github.com/RadeonOpenCompute/ROCK-Kernel-Driver
      * HSA runtime API and runtime for Boltzmann:  https://github.com/RadeonOpenCompute/ROCR-Runtime

* Traditional HSA driver + APU (Kaveri)


3.2. GPU Cards
^^^^^^^^^^^^^^

* Radeon R9 Nano
* Radeon R9 FuryX
* Radeon R9 Fury
* Kaveri and Carizo APU


3.3. Desktop System 
^^^^^^^^^^^^^^^^^^^

* Supermicro SYS-7048GR-TR  Tower 4 W9100 GPU
* ASUS X99-E WS motherboard with 4 AMD FirePro W9100
* Gigabyte GA-X79S 2 AMD FirePro W9100 GPU’s


3.4. Server System 
^^^^^^^^^^^^^^^^^^

* Supermicro SYS 2028GR-THT  6 R9 NANO
* Supermicro SYS-1028GQ-TRT 4 R9 NANO
* Supermicro SYS-7048GR-TR Tower 4 R9 NANO

************************
4. Installation steps
************************
-------------------------------------------------------------------------------------------------------------------------------------------

The following are the steps to use the library

      * Boltzmann Driver and Runtime installation (if not done until now)

      * Compiler installation.

      * Library installation.

4.1 Boltzmann Driver and Runtime Installation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

     a. Downloading the kernel binaries from the repo

        ``git clone https://github.com/RadeonOpenCompute/ROCK-Kernel-Driver.git``

     b. Go to the top of the repo

        ``cd ROCK-Kernel-Driver``

     c. Configure udev to allow any user to access /dev/kfd.
        As root, use a text editor to create /etc/udev/rules.d/kfd.rules
        containing one line: KERNEL=="kfd", MODE="0666", Or you could use the following command

        ``echo "KERNEL==\"kfd\", MODE=\"0666\"" | sudo tee /etc/udev/rules.d/kfd.rules``

     d. For Ubuntu, install the kernel and libhsakmt packages using:

        ``sudo dpkg -i packages/ubuntu/*.deb``

     e. Reboot the system to install the new kernel and enable the HSA kernel driver

        ``sudo reboot``

     f. Once done with reboot, one could proceed with runtime installation

        ``git clone https://github.com/RadeonOpenCompute/ROCR-Runtime``

        ``cd ROCR-Runtime/packages/ubuntu``

        ``sudo dpkg -i hsa-runtime-dev-1.0.0-amd64.deb``

        The contents get installed by default under /opt/hsa path


     e. Create a file called hsa.conf in /etc/ld.so.conf.d/ and write "/opt/hsa/lib" in it,
        then run "sudo ldconfig -v" or you could use the following command

        ``echo "/opt/hsa/lib" | sudo tee /etc/ld.so.conf.d/hsa.conf``

        ``sudo ldconfig -v``

4.2 Compiler Installation
^^^^^^^^^^^^^^^^^^^^^^^^^

     a. Install pre-dependency packages

        ``sudo apt-get install cmake git subversion g++ libstdc++-4.8-dev libdwarf-dev libelf-dev libtinfo-dev libc6-dev-i386 gcc-multilib llvm llvm-dev llvm-runtime libc++1 libc++-dev libc++abi1 libc++abi-dev re2c libncurses5-dev``

     b. Download Compiler

        Click `here <https://bitbucket.org/multicoreware/hcc/downloads/hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb>`_

                                              (or)

        wget https://bitbucket.org/multicoreware/hcc/downloads/hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb

     c. Install the compiler

        ``sudo dpkg -i hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb``

Once done with the above steps the compiler headers, binaries and libraries gets installed under /opt system path as ``/opt/hcc`` .

4.3 Library Installation
^^^^^^^^^^^^^^^^^^^^^^^^

    a. Clone the repo

       ``git clone https://bitbucket.org/multicoreware/hcrng.git && cd hcrng``

    b. Modify scripts mode to binary

       ``chmod +x install.sh``

    c. Install library

       ``./install.sh``

    d. Additionally to run the unit test along with installation invoke the following command

       ``./install.sh test=ON``

Once done with the above steps the libhcblas.so and associated headers gets installed under system path.

To uninstall the library, invoke the following series of commands

       ``chmod +x uninstall.sh``

       ``./uninstall.sh``

**************************
5. Generators and prefixes
**************************
-------------------------------------------------------------------------------------------------------------------------------------------

The API is, in large part, the same for every generator, with only the prefix of the type and function names that changes across generators. For example, to use the MRG31k3p generator, one needs to include the corresponding header file (which is normally the lowercase name of the generator with a .h extension on the host or the device) and use type and function names that start with hcrngMrg31k3p:

::

             #include <hcRNG/mrg31k3p.h>
             double foo(hcrngMrg31k3pStream* stream) {
             return hcrngMrg31k3pRandomU01(stream);
             }

The above function just returns a number uniformly distributed in (0,1) generated using the stream passed as its argument. To use the LFSR113 generator instead of MRG31k3p, one must change the include directive and use type and function names that start with hcrngLfsr113:

::

             #include <hcRNG/lfsr113.h>
             double foo(hcrngLfsr113Stream* stream) {
             return hcrngLfsr113RandomU01(stream);
             }

In the generator API reference (given in hcRNG_template.h), the generator-specific part of the prefix is not shown. The hcRNG.h header file declares common function across different generators and also utility library functions. Future versions of the library will support more generic interfaces that work across generators.

5.1 Small examples
^^^^^^^^^^^^^^^^^^

In the examples given below, we use the MRG31k3p from `[4] <bibliography.html>`_. In general, a stream object contains three states: the initial state of the stream (or seed), the initial state of the current substream (by default it is equal to the seed), and the current state. With MRG31k3p, each state is comprised of six 31-bit integers. Each time a random number is generated, the current state advances by one position. There are also functions to reset the state to the initial one, or to the beginning of the current substream, or to the start of the next substream. Streams can be created and manipulated in arrays of arbitrary sizes. For a single stream, one uses an array of size 1. One can separately declare and allocate memory for an array of streams, create (initialize) the streams, clone them, copy them to preallocated space, etc.


5.2 Using streams on the host
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

We start with a small example in which we just create a few streams, then use them to generate numbers on the host computer and compute some quantity. This could be done as well by using only a single stream, but we use more just for the purpose of illustration.

The code includes the header for the MRG31k3p RNG.

::
   
     #include <hcRNG/mrg31k3p.h>

We create an array of two streams named streams and a single stream named single.

:: 

     hcrngMrg31k3pStream* streams = hcrngMrg31k3pCreateStreams(NULL, 2, NULL, NULL);
     hcrngMrg31k3pStream* single = hcrngMrg31k3pCreateStreams(NULL, 1, NULL, NULL);

Then we repeat the following 100 times: we generate a uniform random number in (0,1) and an integer in {1,…,6}, and compute the indicator that the product is less than 2.

::

     int count = 0;
     for (int i = 0; i < 100; i++) {
       double u = hcrngMrg31k3pRandomU01(&streams[i % 2]);
       int x = hcrngMrg31k3pRandomInteger(single, 1, 6);
       if (x * u < 2) count++;
     }

The uniform random numbers over (0,1) are generated by alternating the two streams from the array. We then print the average of those indicators.

::

     printf("Average of indicators = %f\n", (double)count / 100.0);

5.3 Using streams in work items
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In our second example, we create an array of streams and use them in work items that execute in parallel on a GPU device, one distinct stream per work item. Note that it is also possible (and sometimes useful) to use more than one stream per work item. We show only fragments of the code, to illustrate what we do with the streams. This code is only for illustration; the program does no useful computation.

In the host code, we first include the hcRNG header for the MRG31k3p RNG:

::

    #include <hcRNG/mrg31k3p.h>

Now suppose we have an integer variable numWorkItems that indicates the number of work items we want to use. We create an array of numWorkItems streams (and allocate memory for both the array and the stream objects). The creator returns in the variable streamBufferSize the size of the buffer that this array occupies (it depends on how much space is required to store the stream states), and an error code.

::

    size_t streamBufferSize;
    hcrngMrg31k3pStream* streams = hcrngMrg31k3pCreateStreams(NULL, numWorkItems, &streamBufferSize, (hcrngStatus *)&err);
    

Then we create an HCC buffer of size streamBufferSize and fill it with a copy of the array of streams, to pass to the device. We also create and pass a buffer that will be used by the device to return an array of numWorkItems values of type double.

Create buffer to transfer streams to the device.
   
::               

   hcrngMrg31k3pStream* buf_in = hc::am_alloc(sizeof(hcrngMrg31k3pStream) * numWorkItems, acc[1], 0);
   hc::am_copy(streams_buffer, streams, numWorkItems * sizeof(hcrngMrg31k3pStream));

Create buffer to transfer output back from the device.

::

   float* buf_out = hc::am_alloc(sizeof(float) * numberCount, acc[1], 0);

Then depending on the type of arguments, we call the kernel, 

::
   
  hcrngMrg31k3pDeviceRandomU01Array_single(accl_view, numWorkItems, buf_in, numberCount, buf_out); 

The host can then recover the array of size numWorkItems that contains these outputs(numberCount).
RNG-Specific API's
  
::

  hc::am_copy(RandomOutput, buf_out, numberCount * sizeof(float));


hcRNG_template.h describes the random streams API as it is intended to be implemented using different types of RNG's or even using quasi-Monte Carlo (QMC) point sets.

In the description of this API, every data type and function name is assigned the prefix hcrng. It is understood that, in the implementation for each RNG type, the prefix hcrng is to be expanded with another prefix that indicates the type of RNG (or other method) used.

As this API is not polymorphic, replacing an RNG type with another one in client code requires changing the code to match hcRNG function names and data types to match those of the replacement RNG. We also intend to propose a generic (in the polymorphic sense) interface to the hcRNG library.

5.4 Stream Objects and Stream States
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The library defines, among others, two closely related types of structures: stream objects (hcrngStream) and stream states (hcrngStreamState). The definitions of both structures depend on the specific type of RNG that they pertain to. Stream states correspond to the seeds of conventional RNG's, to counter values in counter-based RNG's, or to point and coordinate indices in QMC methods. Normally, the client should not deal with stream states directly, but use instead the higher-level stream objects. Stream objects are intended to store several stream states: the current and initial stream states, but also current substream state when support for substreams is available. Stream objects may also store other properties of the RNG, such as encryption keys for cryptography-based RNG's.

5.5 Arrays of Stream Objects
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Many functions are defined only for arrays of stream objects and not for single stream objects. It is always possible to use these functions for single stream objects by specifying a unit array size.

5.6 Defining Preprocessors
^^^^^^^^^^^^^^^^^^^^^^^^^^

When a kernel is called, the stream states it needs are normally passed by the host and stored in global memory. If default settings are not suitable for the user's needs, optional library behavior can be selected by defining specific preprocessor macros before including the hcRNG header. For example, to enable substreams support on the device while using the MRG31k3p generator, use:

::

     #define HCRNG_ENABLE_SUBSTREAMS


.. toctree::
   :maxdepth: 2

   bibliography

.. Index::
   index
