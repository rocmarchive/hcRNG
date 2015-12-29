#!/bin/bash -e
#This script is invoked to test all generators of the hcRNG library 
#Preliminary version

# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

#Move to library build
cd $current_work_dir/../lib/build/linux

#Invoke build script
sh build.sh

# Install library
sudo make install

#Move to test build
cd $current_work_dir/build/linux

#Invoke build script
sh build.sh

# build test src
make

# Move to test bin
cd $current_work_dir/build/linux/src/bin/

echo "TEST PASSED" >> testlog_temp.txt
#Test generators
path2mrg31k3psingle="$current_work_dir/build/linux/src/bin/mrg31k3pSingle"
path2mrg31k3pdouble="$current_work_dir/build/linux/src/bin/mrg31k3pDouble"
path2mrg32k3asingle="$current_work_dir/build/linux/src/bin/mrg32k3aSingle"
path2mrg32k3adouble="$current_work_dir/build/linux/src/bin/mrg32k3aDouble"
path2lfsr113single="$current_work_dir/build/linux/src/bin/lfsr113Single"
path2lfsr113double="$current_work_dir/build/linux/src/bin/lfsr113Double"
path2philox432single="$current_work_dir/build/linux/src/bin/philox432Single"
path2philox432double="$current_work_dir/build/linux/src/bin/philox432Double"

runcmd1="$path2mrg31k3psingle >> testlog.txt"
eval $runcmd1

runcmd2="$path2mrg31k3pdouble >> testlog.txt"
eval $runcmd2

runcmd3="$path2mrg32k3asingle >> testlog.txt"
eval $runcmd3

runcmd4="$path2mrg32k3adouble >> testlog.txt"
eval $runcmd4

runcmd5="$path2lfsr113single  >> testlog.txt"
eval $runcmd5

runcmd6="$path2lfsr113double  >> testlog.txt"
eval $runcmd6

runcmd7="$path2philox432single >> testlog.txt"
eval $runcmd7

runcmd8="$path2philox432double >> testlog.txt"
eval $runcmd8

echo "TEST PASSED" >> testlog.txt
DIFF=$(diff testlog.txt testlog_temp.txt)
if [ "$DIFF" != "" ]
then
    echo "TEST FAILED"
else
    echo "All Unit Tests Passed!"
fi
rm testlog*
