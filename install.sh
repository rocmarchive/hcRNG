# This script is invoked to install the hcRNG library and test sources
# Preliminary version

# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

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

# TODO: ADD More options to perform benchmark and testing
