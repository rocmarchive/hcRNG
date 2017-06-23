#!/bin/bash -e
# This script is invoked to uninstall the hcRNG library and test sources
# Preliminary version

# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`
install=0

# Help menu
print_help() {
cat <<-HELP
=============================================================================================================================
Please provide the following arguments:

  ${green}--deepclean${reset}  To clean the files under /opt/rocm/hcrng  Requires sudo perms.

=============================================================================================================================
Usage: ./clean.sh --deepclean 
=============================================================================================================================
HELP
exit 0
}

while [ $# -gt 0 ]; do
  case "$1" in
    --deepclean)
      deepclean="1"
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

# Remove build
rm -rf $current_work_dir/build

# Remove files under /opt/rocm/hcrng
if [ "$deepclean" = "1" ]; then
     rm -rf /opt/rocm/hcrng
     rm -f /opt/rocm/lib/libhcrng.so  
     rm -f /opt/rocm/lib/libhiprng.so
fi

