#!/bin/bash -e
#This script is invoked to test all generators of the hcRNG library 
#Preliminary version

# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

# Move to src test bin
working_dir="$current_work_dir/../build/test/src/bin/"
cd $working_dir

echo "TEST PASSED" >> testlog_temp.txt
#Test generators
path2mrg31k3psingle="$working_dir/mrg31k3pSingle"
path2mrg31k3pdouble="$working_dir/mrg31k3pDouble"
path2mrg32k3asingle="$working_dir/mrg32k3aSingle"
path2mrg32k3adouble="$working_dir/mrg32k3aDouble"
path2lfsr113single="$working_dir/lfsr113Single"
path2lfsr113double="$working_dir/lfsr113Double"
path2philox432single="$working_dir/philox432Single"
path2philox432double="$working_dir/philox432Double"

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
    echo "Functionality Check ---- [ FAILED ]"
    rm testlog_temp.txt
else
    echo "Functionality Check ---- [ PASSED ]"
    rm testlog*
fi

# Move to gtest bin
working_dir1="$current_work_dir/../build/test/unit/bin/"
cd $working_dir1

#Gtest functions
path2StreamCreator="$working_dir1/test_Stream_Creator"
path2Streams="$working_dir1/test_Streams"
path2Substreams="$working_dir1/test_Substreams"
path2Randomarray="$working_dir1/test_Randomarray"

runcmd1="$path2StreamCreator >> gtestlog.txt"
eval $runcmd1

runcmd2="$path2Streams >> gtestlog.txt"
eval $runcmd2

runcmd3="$path2Substreams >> gtestlog.txt"
eval $runcmd3

runcmd4="$path2Randomarray >> gtestlog.txt"
eval $runcmd4

Log_file="$working_dir1/gtestlog.txt"
if grep -q FAILED "$Log_file"; 
then
    echo "GTEST               ---- [ FAILED ]"   
else
    echo "GTEST               ---- [ PASSED ]"
fi
rm $working_dir1/gtestlog.txt
