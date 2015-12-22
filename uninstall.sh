# This script is invoked to uninstall the hcRNG library and test sources
# Preliminary version

# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

#Move to library build
cd $current_work_dir/lib/build/linux

#Invoke clean script
sh clean.sh

#Move to test build
cd $current_work_dir/test/build/linux

#Invoke clean script
sh clean.sh

#Remove installed libraries and headers
sudo rm /usr/lib/libhcrng.so
sudo rm -rf /usr/local/include/hcRNG

#Remove CMake progress folders
DIRECTORY=$CURRENTDIR/lib/build/linux/CMakeFiles
if [ -d "$DIRECTORY" ]; then
  cd $CURRENTDIR/lib/build/linux/
  sudo rm -rf CMakeFiles
fi

# TODO: ADD More options
