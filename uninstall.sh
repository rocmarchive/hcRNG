#!/bin/bash -e
# This script is invoked to uninstall the hcRNG library and test sources
# Preliminary version

# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

# Remove system wide installed lib and headers
if [ -f "$current_work_dir/build/install_manifest.txt" ]
then
sudo xargs rm < $current_work_dir/build/install_manifest.txt
fi

# Remove build
sudo rm -rf $current_work_dir/build

# TODO: ADD More options
