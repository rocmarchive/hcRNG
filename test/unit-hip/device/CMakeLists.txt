cmake_minimum_required(VERSION 2.8)

# HIP_PATH
IF(NOT DEFINED $ENV{HIP_PATH})
  SET(HIP_PATH "/opt/rocm/hip")
ELSE()
  SET(HIP_PATH $ENV{HIP_PATH})
ENDIF()

SET(HIP_SUPPORT $ENV{HIP_SUPPORT})

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${HIP_PATH}/cmake)

add_subdirectory(device_normal)
add_subdirectory(device_uniform)
