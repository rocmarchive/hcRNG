#!/bin/bash -e
# This script is invoked to install the hcRNG library and test sources
# Preliminary version

# CHECK FOR COMPILER PATH
if [ ! -z $MCWHCCBUILD]; 
then
  if [ -x "$MCWHCCBUILD/compiler/bin/clang++" ]; 
  then
    cmake_c_compiler="$MCWHCCBUILD/compiler/bin/clang"
    cmake_cxx_compiler="$MCWHCCBUILD/compiler/bin/clang++"
  fi

elif [ -x "/opt/hcc/bin/clang++" ]; 
then
  cmake_c_compiler="/opt/hcc/bin/clang"
  cmake_cxx_compiler="/opt/hcc/bin/clang++"
else
  echo "Clang compiler not found"
  exit 1
fi

# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

# Inputs converted to smallcase format
input1=$1
var1=${input1,,}

# If number of arguments = 0
if [ "x$var1" = "x" ]; then
   var1="test=off"
fi

# MAKE BUILD DIR
mkdir $current_work_dir/build
mkdir $current_work_dir/build/lib
mkdir $current_work_dir/build/test
#mkdir $current_work_dir/build/test/bin/

# SET BUILD DIR
build_dir=$current_work_dir/build

# change to library build
cd $build_dir/lib

# Cmake and make libhcfft: Install hcFFT
cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir
sudo make install

# Build Tests
cd $build_dir/test/ && cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir/test/ 
mkdir $current_work_dir/build/test/src/bin/
make

# Simple test to confirm installation
#$build_dir/test/src/fft 2 12

# TODO: ADD More options to perform benchmark and testing





#if [ "$var1" = "test=off" ]; then
#Move to library build
#  cd $current_work_dir/lib/build/linux



#elif [ "$var1" = "test=on" ]; then
#Move to test folder
#  cd $current_work_dir/test/
  
#Invoke test script
#  ./test.sh

#fi
#echo "HCRNG Installation Completed!"


# TODO: ADD More options to perform benchmark and testing
