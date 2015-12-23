#!/bin/bash -e
#This script is invoked to install the hcRNG library and test sources
# Preliminary version

# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

#Inputs converted to smallcase format
input1=$1
var1=${input1,,}

#If number of arguments = 0
if [ "x$var1" = "x" ]; then
   var1="test=off"
fi

if [ "$var1" = "test=off" ]; then
#Move to library build
  cd $current_work_dir/lib/build/linux

#Invoke build script
  sh build.sh

# Install library
  sudo make install

#Move to test build
  cd $current_work_dir/test/build/linux

#Invoke build script
  sh build.sh

# build test src
  make

elif [ "$var1" = "test=on" ]; then
#Move to test folder
  cd $current_work_dir/test/
  
#Invoke test script
  ./test.sh

fi
echo "HCRNG Installation Completed!"


# TODO: ADD More options to perform benchmark and testing
