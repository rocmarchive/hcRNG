#!/bin/bash -e 
# This script is invoked to install the hcRNG library and test sources

# CHECK FOR COMPILER PATH
if [ ! -z $HCC_HOME ]
then
  platform="hcc"
  if [ -x "$HCC_HOME/bin/clang++" ]
  then
    cmake_c_compiler=$HCC_HOME/bin/clang
    cmake_cxx_compiler=$HCC_HOME/bin/clang++
  fi
elif [ -x "/opt/rocm/hcc/bin/clang++" ]
then
  platform="hcc"
  cmake_c_compiler=/opt/rocm/hcc/bin/clang
  cmake_cxx_compiler=/opt/rocm/hcc/bin/clang++
elif [ -x "/usr/local/cuda/bin/nvcc" ];
then
  platform="nvcc"
  cmake_c_compiler="/usr/bin/gcc"
  cmake_cxx_compiler="/usr/bin/g++"
else
  echo "Neither clang  or NVCC compiler found"
  echo "Not an AMD or NVCC compatible stack"
  exit 1
fi

# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$current_work_dir/build/lib/src

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

# Help menu
print_help() {
cat <<-HELP
=============================================================================================================================
This script is invoked to build hcRNG library and test sources. Please provide the following arguments:

  ${green}--test${reset}    Test to enable the library testing. 

=============================================================================================================================
Usage: ./build.sh --test=on (or off) 
=============================================================================================================================
Example: 
(1) ${green}./build.sh --test=on ${reset}

=============================================================================================================================
HELP
exit 0
}

while [ $# -gt 0 ]; do
  case "$1" in
    --test=*)
      testing="${1#*=}"
      ;;
    --hip_so=*)
      hip_so="${1#*=}"
      ;;
    --help) print_help;;
    *)
      printf "************************************************************\n"
      printf "* Error: Invalid arguments, run --help for valid arguments.*\n"
      printf "************************************************************\n"
      exit 1
  esac
  shift
done


set +e
# MAKE BUILD DIR
mkdir -p $current_work_dir/build
mkdir -p $current_work_dir/build/lib
set -e

# SET BUILD DIR
build_dir=$current_work_dir/build
#change to library build
cd $build_dir


if [ "$platform" = "hcc" ]; then
  
  # Cmake and make libhcRNG: Install hcRNG
  if ( [ "$hip_so" = "on" ] ); then
    cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DHIP_SHARED_OBJ=ON -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir
  else
    cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir
  fi
  make package
  make

  #Test=OFF (Build library and tests)
  if ( [ -z $testing ] ) || ( [ "$testing" = "off" ] ); then
    echo "${green}HCRNG Installation Completed!${reset}"
  # Test=ON (Build and test the library)
  elif ( [ "$testing" = "on" ] ); then
    set +e
    # MAKE BUILD DIR
    mkdir $current_work_dir/build/test
    mkdir -p $current_work_dir/build/test/unit-hip/common/bin/
    mkdir -p $current_work_dir/build/test/unit-hip/normal/bin/
    mkdir -p $current_work_dir/build/test/unit-hip/uniform/bin/
    set -e

    # Build Tests
    if ( [ "$hip_so" = "on" ] ); then
      cd $build_dir/test/ && cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DHIP_SHARED_OBJ=ON -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir/test/
    else
      cd $build_dir/test/ && cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir/test/
    fi
    make

    #Move to test folder
    cd $current_work_dir/test/
    #Invoke test script
     printf "* UNIT *\n"
     printf "********\n"
    ./test.sh
    
    chmod +x $current_work_dir/test/unit-hip/test.sh
    cd $current_work_dir/test/unit-hip/
    # Invoke hip unit test script
     printf "* UNIT HIP TESTS*\n"
     printf "*****************\n"
    ./test.sh
  fi
fi

if [ "$platform" = "nvcc" ]; then
  # Cmake and make libhipRNG: Install hipRNG
  if ( [ "$hip_so" = "on" ] ); then
    cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DHIP_SHARED_OBJ=ON -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir
    
    make package
    make
    echo "${green}HIPRNG Build Completed!${reset}"
  fi
  
  if ( [ "$testing" = "on" ] ); then
    set +e
    # MAKE BUILD DIR
    mkdir $current_work_dir/build/test
    mkdir -p $current_work_dir/build/test/unit-hip/common/bin/
    mkdir -p $current_work_dir/build/test/unit-hip/normal/bin/
    mkdir -p $current_work_dir/build/test/unit-hip/uniform/bin/
    set -e

    # Build Tests
    if ( [ "$hip_so" = "on" ] ); then
      cd $build_dir/test/ && cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DHIP_SHARED_OBJ=ON -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir/test/
    else
      cd $build_dir/test/ && cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir/test/
    fi
    make

    chmod +x $current_work_dir/test/unit-hip/test.sh
    cd $current_work_dir/test/unit-hip/
     printf "* UNIT HIP TESTS*\n"
    # Invoke hip unit test script
    ./test.sh
  fi
fi
# TODO: ADD More options to perform benchmark
