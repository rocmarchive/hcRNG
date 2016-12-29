#!/bin/bash
#This script is invoked to test all generators of the hcfft library
#Preliminary version

# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

test_common_file=($current_work_dir/common/*.cpp)
numtests=${#test_common_file[@]}

## now loop through the above array
  for (( i=0; i<numtests; i++ ));  
  do
    test_common=$(basename "${test_common_file[$i]}" .cpp)
    working_dir1="$current_work_dir/../../build/test/unit-hip/common/bin"
    cd $working_dir1
    if [ ! -d "errlog" ]; then
      mkdir "errlog"
    fi
    errlogdir="${working_dir1}/errlog"

    #Gtest functions
    unittest="${working_dir1}/${test_common}"

    runcmd1="$unittest >> gtestlog.txt"
    eval $runcmd1

    Log_file="$working_dir1/gtestlog.txt"
    if [ ! -s "$Log_file" ]; then
      echo "${red}GTEST IS NOT WORKING....${reset}"
    else
      if grep -q FAILED "$Log_file";
      then
        echo "${red}hip_${test_common}            ----- [ FAILED ]${reset}"
        mv "${working_dir1}/gtestlog.txt" "${errlogdir}/${test_common}.txt" 
      elif grep -q PASSED "$Log_file";
      then
        echo "${green}hip_${test_common}          ----- [ PASSED ]${reset}"
        rm -f $working_dir1/gtestlog.txt
      fi
    fi
  done

test_normal_file=($current_work_dir/normal/*.cpp)
numtests=${#test_normal_file[@]}

## now loop through the above array
  for (( i=0; i<numtests; i++ ));  
  do
    test_normal=$(basename "${test_normal_file[$i]}" .cpp)
    working_dir1="$current_work_dir/../../build/test/unit-hip/normal/bin"
    cd $working_dir1
    if [ ! -d "errlog" ]; then
      mkdir "errlog"
    fi
    errlogdir="${working_dir1}/errlog"

    #Gtest functions
    unittest="${working_dir1}/${test_normal}"

    runcmd1="$unittest >> gtestlog.txt"
    eval $runcmd1

    Log_file="$working_dir1/gtestlog.txt"
    if [ ! -s "$Log_file" ]; then
      echo "${red}GTEST IS NOT WORKING....${reset}"
    else
      if grep -q FAILED "$Log_file";
      then
        echo "${red}hip_${test_normal}            ----- [ FAILED ]${reset}"
        mv "${working_dir1}/gtestlog.txt" "${errlogdir}/${test_normal}.txt" 
      elif grep -q PASSED "$Log_file";
      then
        echo "${green}hip_${test_normal}          ----- [ PASSED ]${reset}"
        rm -f $working_dir1/gtestlog.txt
      fi
    fi
  done

test_uniform_file=($current_work_dir/uniform/*.cpp)
numtests=${#test_uniform_file[@]}

## now loop through the above array
  for (( i=0; i<numtests; i++ ));  
  do
    test_uniform=$(basename "${test_uniform_file[$i]}" .cpp)
    working_dir1="$current_work_dir/../../build/test/unit-hip/uniform/bin"
    cd $working_dir1
    if [ ! -d "errlog" ]; then
      mkdir "errlog"
    fi
    errlogdir="${working_dir1}/errlog"

    #Gtest functions
    unittest="${working_dir1}/${test_uniform}"

    runcmd1="$unittest >> gtestlog.txt"
    eval $runcmd1

    Log_file="$working_dir1/gtestlog.txt"
    if [ ! -s "$Log_file" ]; then
      echo "${red}GTEST IS NOT WORKING....${reset}"
    else
      if grep -q FAILED "$Log_file";
      then
        echo "${red}hip_${test_uniform}            ----- [ FAILED ]${reset}"
        mv "${working_dir1}/gtestlog.txt" "${errlogdir}/${test_uniform}.txt" 
      elif grep -q PASSED "$Log_file";
      then
        echo "${green}hip_${test_uniform}          ----- [ PASSED ]${reset}"
        rm -f $working_dir1/gtestlog.txt
      fi
    fi
  done
